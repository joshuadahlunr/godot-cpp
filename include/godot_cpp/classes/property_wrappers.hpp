#ifndef GODOT_PROPERTY_WRAPPERS_HPP
#define GODOT_PROPERTY_WRAPPERS_HPP

#include <godot_cpp/classes/properties.hpp>

// Helper that defines all the different permutations of access to a function based on if the property has a getter (and a setter) defined
#define GODOT_PROPERTY_WRAPPED_FUNCTION(name, self) template<typename... Args>\
	auto name(Args... args) {\
		static_assert(getable<self>, "Trying to call a wrapped function on a set only property!");\
\
		auto temp = get();\
		if constexpr (std::is_same_v<typename godot::detail::ReturnType<&self::value_type::name>::type, void>){\
			temp.name(std::forward<Args>(args)...);\
			static_assert(godot::detail::IsMethodConst<&self::value_type::name>::value || setable<self>, "Trying to call a non-const function on a read only property!");\
			if constexpr (!godot::detail::IsMethodConst<&self::value_type::name>::value && setable<self>) set(temp);\
		} else {\
			auto ret = temp.name(std::forward<Args>(args)...);\
			static_assert(godot::detail::IsMethodConst<&self::value_type::name>::value || setable<self>, "Trying to call a non-const function on a read only property!");\
			if constexpr (!godot::detail::IsMethodConst<&self::value_type::name>::value && setable<self>) set(temp);\
			return ret;\
		}\
	}\
	template<typename... Args>\
	auto name(Args... args) const {\
		static_assert(getable<self>, "Trying to call a wrapped function on a set only property!");\
\
		auto temp = get();\
		if constexpr (std::is_same_v<typename godot::detail::ReturnType<&self::value_type::name>::type, void>)\
			temp.name(std::forward<Args>(args)...);\
		else return temp.name(std::forward<Args>(args)...);\
	}

// Helper that creates the different permutations of a property that copies if there is a getter or a setter for the parent property
#define GODOT_PROPERTY_WRAPPED_PROPERTY_NO_GET_SET(type, name, self) auto name() {\
	if constexpr (getsetable<self>) return Property<type, &self::get_##name, &self::set_##name>{this};\
	else if constexpr (getable<self>) return Property<type, &self::get_##name, nullptr>{this};\
	else if constexpr (setable<self>) return Property<type, nullptr, &self::set_##name>{this};\
	else return Property<type, nullptr, nullptr>{this};\
}\
const auto name() const {\
	if constexpr (getsetable<self> || getable<self>) return Property<type, &self::get_##name, nullptr>{this};\
	else return Property<type, nullptr, nullptr>{this};\
}

// Helper that creates the different permutations of a property that copies if there is a getter or a setter for the parent property
// Also defines get_name and set_name methods which get and set the property 
#define GODOT_PROPERTY_WRAPPED_PROPERTY(type, name, self) type get_##name() const requires getable<self> { return get().name; }\
	type set_##name(const type& value) requires getsetable<self> { auto temp = get(); temp.name = value; set(temp); return temp.name; }\
	GODOT_PROPERTY_WRAPPED_PROPERTY_NO_GET_SET(type, name, self)

// Helper that creates the different permutations of a property that copies if there is a getter or a setter for the parent property
// Also defines get_name and set_name methods which get and set the property (assumes that the referenced entity is a property which needs to them be called)
#define GODOT_PROPERTY_WRAPPED_PROPERTY_CALL(type, name, self) type get_##name() const requires getable<self> { return get().name(); }\
	type set_##name(type value) requires getsetable<self> { auto temp = get(); temp.name() = value; set(temp); return temp.name(); }\
	GODOT_PROPERTY_WRAPPED_PROPERTY_NO_GET_SET(type, name, self)

#endif // GODOT_PROPERTY_WRAPPERS_HPP