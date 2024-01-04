#ifndef GODOT_PROPERTIES_HPP
#define GODOT_PROPERTIES_HPP

#include <concepts>
#include <type_traits>
#include <utility>

namespace godot {
namespace detail {
	template<auto F>
	struct ObjectType { using type = void; };

	template<typename Object, typename Ret, typename... Args, Ret (Object::*F)(Args...)>
	struct ObjectType<F> { using type = Object; };

	template<typename Object, typename Ret, typename... Args, Ret (Object::*F)(Args...) const>
	struct ObjectType<F> { using type = Object; };

	template<auto F>
	struct ReturnType { using type = void; };

    template<typename Ret, typename... Args, Ret (*F)(Args...)>
	struct ReturnType<F> { using type = Ret; };

	template<typename Object, typename Ret, typename... Args, Ret (Object::*F)(Args...)>
	struct ReturnType<F> { using type = Ret; };

	template<typename Object, typename Ret, typename... Args, Ret (Object::*F)(Args...) const>
	struct ReturnType<F> { using type = Ret; };

	template<auto F>
	struct FirstArgumentType { using type = void; };

	template<typename Object, typename Ret, typename Arg0, typename... Args, Ret (Object::*F)(Arg0, Args...)>
	struct FirstArgumentType<F> { using type = Arg0; };

	template<typename Object, typename Ret, typename Arg0, typename... Args, Ret (Object::*F)(Arg0, Args...) const>
	struct FirstArgumentType<F> { using type = Arg0; };

	template<auto F>
	struct IsMemberFunction { static constexpr bool value = false; };

	template<typename Object, typename Ret, typename... Args, Ret (Object::*F)(Args...)>
	struct IsMemberFunction<F> { static constexpr bool value = true; };

	template<typename Object, typename Ret, typename... Args, Ret (Object::*F)(Args...) const>
	struct IsMemberFunction<F> { static constexpr bool value = true; };

    template<auto F>
	struct IsMethodConst { static constexpr bool value = false; };

	template<typename Object, typename Ret, typename... Args, Ret (Object::*F)(Args...)>
	struct IsMethodConst<F> { static constexpr bool value = false; };

	template<typename Object, typename Ret, typename... Args, Ret (Object::*F)(Args...) const>
	struct IsMethodConst<F> { static constexpr bool value = true; };
} // detail

template<typename T>
concept getable = requires(T t) { t.get(); };

template<typename T>
concept setable = requires(T t, T::value_type v) { t.set(v); };

template<typename T>
concept getsetable = getable<T> && setable<T>;

template<typename F, typename... Args>
concept returns_void = std::invocable<F> && requires (F f, Args... args) { { f(args...) } -> std::same_as<void>; };

template<typename Derived>
struct PropertyOperations {
	Derived& underlying() { return *(Derived*)this; }

	// Math operations
	template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t + o; }))
	auto operator+(Other o) { return underlying().get() + o; }

    auto operator+() requires (getable<Derived> && (requires (Derived::value_type t) { +t; }))
        { return +underlying().get(); }

    template<typename Other>
	requires (getsetable<Derived> && (requires (Derived::value_type t, Other o) { t += o; }))
	Derived& operator+=(Other o) { auto temp = underlying().get(); underlying().set(temp += o); return underlying(); }

	Derived& operator++() requires (getsetable<Derived> && requires (Derived::value_type t) { ++t; })
        { auto temp = underlying().get(); underlying().set(++temp); return underlying(); }
    
    Derived& operator++(int) requires (getsetable<Derived> && requires (Derived::value_type t) { t++; })
        { auto temp = underlying().get(); temp++; underlying().set(temp); return underlying(); }

	template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t * o; }))
	auto operator*(Other o) { return underlying().get() * o; }

    template<typename Other>
    requires (getsetable<Derived> && (requires (Derived::value_type t, Other o) { t *= o; }))
	Derived& operator*=(Other o) { auto temp = underlying().get(); underlying().set(temp *= o); return underlying(); }

	template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t / o; }))
	auto operator/(Other o) { return underlying().get() / o; }

    template<typename Other>
    requires (getsetable<Derived> && (requires (Derived::value_type t, Other o) { t /= o; }))
	Derived& operator/=(Other o) { auto temp = underlying().get(); underlying().set(temp /= o); return underlying(); }

	template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t - o; }))
	auto operator-(Other o) { return underlying().get() - o; }

    auto operator-() requires (getable<Derived> && (requires (Derived::value_type t) { -t; }))
        { return -underlying().get(); }

    template<typename Other>
    requires (getsetable<Derived> && (requires (Derived::value_type t, Other o) { t -= o; }))
	Derived& operator-=(Other o) { auto temp = underlying().get(); underlying().set(temp -= o); return underlying(); }

    Derived& operator--() requires (getsetable<Derived> && (requires (Derived::value_type t) { --t; }))
        { auto temp = underlying().get(); underlying().set(--temp); return underlying(); }
    
    Derived& operator--(int) requires (getsetable<Derived> && (requires (Derived::value_type t) { t--; }))
        { auto temp = underlying().get(); temp--; underlying().set(temp); return underlying(); }

	template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t % o; }))
	auto operator%(Other o) { return underlying().get() % o; }

    template<typename Other>
    requires (getsetable<Derived> && (requires (Derived::value_type t, Other o) { t %= o; }))
	Derived& operator%=(Other o) { auto temp = underlying().get(); underlying().set(temp %= o); return underlying(); }

    // Bitwise operations
    template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t ^ o; }))
	auto operator^(Other o) { return underlying().get() ^ o; }

    template<typename Other>
    requires (getsetable<Derived> && (requires (Derived::value_type t, Other o) { t ^= o; }))
	Derived& operator^=(Other o) { auto temp = underlying().get(); underlying().set(temp ^= o); return underlying(); }

    template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t & o; }))
	auto operator&(Other o) { return underlying().get() & o; }

    template<typename Other>
    requires (getsetable<Derived> && (requires (Derived::value_type t, Other o) { t &= o; }))
	Derived& operator&=(Other o) { auto temp = underlying().get(); underlying().set(temp &= o); return underlying(); }

    template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t | o; }))
	auto operator|(Other o) { return underlying().get() | o; }

    template<typename Other>
    requires (getsetable<Derived> && (requires (Derived::value_type t, Other o) { t |= o; }))
	Derived& operator|=(Other o) { auto temp = underlying().get(); underlying().set(temp |= o); return underlying(); }

	auto operator~() requires (getable<Derived> && requires (Derived::value_type t) { ~t; })
        { return ~underlying().get(); }

    auto operator!() requires (getable<Derived> && requires (Derived::value_type t) { !t; })
        { return !underlying().get(); }

    // Shift operators
    template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t << o; }))
	auto operator<<(Other o) { return underlying().get() << o; }

    template<typename Other>
    requires (getsetable<Derived> && (requires (Derived::value_type t, Other o) { t <<= o; }))
	Derived& operator<<=(Other o) { auto temp = underlying().get(); underlying().set(temp <<= o); return underlying(); }

    template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t >> o; }))
	auto operator>>(Other o) { return underlying().get() >> o; }

    template<typename Other>
    requires (getsetable<Derived> && (requires (Derived::value_type t, Other o) { t >>= o; }))
	Derived& operator>>=(Other o) { auto temp = underlying().get(); underlying().set(temp >>= o); return underlying(); }

	// Relational operations
	template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t == o; }))
	auto operator==(Other o) { return underlying().get() == o; }

	template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t != o; }))
	auto operator!=(Other o) { return underlying().get() != o; }

	template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t >= o; }))
	auto operator>=(Other o) { return underlying().get() >= o; }

	template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t <= o; }))
	auto operator<=(Other o) { return underlying().get() <= o; }

	template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t > o; }))
	auto operator>(Other o) { return underlying().get() > o; }

	template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t < o; }))
	auto operator<(Other o) { return underlying().get() < o; }

    // Function call
    template<typename... Args>
	requires (getable<Derived> && !returns_void<typename Derived::value_type, Args...>)
	auto operator()(Args... args) { auto temp = underlying().get(); auto ret = temp(std::forward<Args>(args)...); set(temp); return ret; }

    template<typename... Args>
	requires (getable<Derived> && returns_void<typename Derived::value_type, Args...>)
	void operator()(Args... args) { auto temp = underlying().get(); temp(std::forward<Args>(args)...); set(temp); }

    // Subscript
#ifdef __cpp_multidimensional_subscript
    template<typename... Args>
	requires (getable<Derived> && (requires (Derived::value_type t, Args... args) { t[args...]; }))
	auto operator[](Args... args) { auto temp = underlying().get(); auto ret = temp[std::forward<Args>(args)...]; set(temp); return ret; }
#else // __cpp_multidimensional_subscript
    template<typename Other>
	requires (getable<Derived> && (requires (Derived::value_type t, Other o) { t[o]; }))
	auto operator[](Other o) { auto temp = underlying().get(); auto ret = temp[o]; set(temp); return ret; }
#endif // __cpp_multidimensional_subscript
};


#define PROPERTY_CORE(Getter, Setter) public:\
	static constexpr bool hasGetter = !std::is_same_v<decltype(Getter), std::nullptr_t>;\
	static constexpr bool hasSetter = !std::is_same_v<decltype(Setter), std::nullptr_t>;\
	static constexpr bool setterReturns = !std::is_same_v<typename godot::detail::ReturnType<Setter>::type, void>;\
	static_assert(hasGetter || hasSetter, "Properties must have at least a getter or a setter!");\
	static_assert(!hasGetter || std::is_convertible_v<std::remove_cvref_t<std::remove_cvref_t<typename godot::detail::ReturnType<Getter>::type>>, T>, "The getter function must return T");\
	static_assert(!hasSetter || std::is_convertible_v<std::remove_cvref_t<std::remove_cvref_t<typename godot::detail::FirstArgumentType<Setter>::type>>, T>, "The setter function must take a T as its first argument");\
	static_assert(!hasGetter || !hasSetter || std::is_convertible_v<typename godot::detail::ObjectType<Getter>::type, typename godot::detail::ObjectType<Setter>::type>, "The getter and setter must belong to the same class!");\
\
protected:\
	using SetterReturn = typename godot::detail::ReturnType<Setter>::type;\
	using Object__ = std::conditional_t<hasGetter, typename godot::detail::ObjectType<Getter>::type, std::conditional_t<hasSetter, typename godot::detail::ObjectType<Setter>::type, void>>;\
	Object__* object;\
\
public:\
	Property(Object__* me) : object(me) { }\
\
	/*template<>\
	Property() { static_assert(!Assert, "Properties must be pointed at an instance of the class their getter/setter is apart of."); }*/\
	Property() {}\
\
	auto operator()() const requires(hasGetter) {\
		return (object->*Getter)();\
	}\
	SetterReturn operator()(T const & value) requires(hasSetter && setterReturns) {\
		return (object->*Setter)(value);\
	}\
	void operator()(T const & value) requires(hasSetter && !setterReturns) {\
		(object->*Setter)(value);\
	}\
\
	auto get() const requires(hasGetter) {\
		return (object->*Getter)();\
	}\
	SetterReturn set(T const & value) requires(hasSetter && setterReturns) {\
		return (object->*Setter)(value);\
	}\
	void set(T const & value) requires(hasSetter && !setterReturns) {\
		(object->*Setter)(value);\
	}\
\
	operator T() const requires(hasGetter) {\
		return (object->*Getter)();\
	}\
	SetterReturn operator=(T const & value) requires(hasSetter && setterReturns) {\
		return (object->*Setter)(value);\
	}\
	void operator=(T const & value) requires(hasSetter && !setterReturns) {\
		(object->*Setter)(value);\
	}\
	Property<T, Getter, nullptr> make_read_only() requires(hasGetter) { return object; }\
	Property<T, nullptr, Setter> make_write_only() requires(hasSetter) { return object; }\
	typedef T value_type;\
	typedef Object__ parent_type;




#define PROPERTY_TEMPLATE_CONSTRAINT(Getter, Setter) requires((detail::IsMemberFunction<Getter>::value || std::is_same_v<decltype(Getter), std::nullptr_t>)\
	&& (detail::IsMemberFunction<Setter>::value || std::is_same_v<decltype(Setter), std::nullptr_t>)\
)

template <class T, auto Getter, auto Setter = nullptr, bool Assert = true> PROPERTY_TEMPLATE_CONSTRAINT(Getter, Setter)
struct Property : PropertyOperations<Property<T, Getter, Setter>> {
	PROPERTY_CORE(Getter, Setter)
};


// Helper that defines all the different permutations of access to a function based on if the property has a getter (and a setter) defined
// #define GODOT_PROPERTY_WRAPPED_FUNCTION(name, self) template<typename... Args> requires (getsetable<self> && !std::is_same_v<detail::ReturnType<&value_type::name>::type, void>) auto name(Args... args) { auto temp = get(); auto ret = temp.name(std::forward<Args>(args)...); set(temp); return ret; }\
// 	template<typename... Args> requires (!getsetable<self> && getable<self> && !std::is_same_v<detail::ReturnType<&value_type::name>::type, void>) auto name(Args... args) const { const auto temp = get(); auto ret = temp.name(std::forward<Args>(args)...); return ret; }\
// 	template<typename... Args> requires (getsetable<self> && std::is_same_v<detail::ReturnType<&value_type::name>::type, void>) void name(Args... args) { auto temp = get(); temp.name(std::forward<Args>(args)...); set(temp); }\
// 	template<typename... Args> requires (!getsetable<self> && getable<self> && std::is_same_v<detail::ReturnType<&value_type::name>::type, void>) void name(Args... args) const { const auto temp = get(); temp.name(std::forward<Args>(args)...); }
#define GODOT_PROPERTY_WRAPPED_FUNCTION(name, self) template<typename... Args>\
	auto name(Args... args) {\
		static_assert(getable<self>, "Trying to call a wrapped function on a set only property!");\
\
		auto temp = get();\
		if constexpr (std::is_same_v<godot::detail::ReturnType<&self::value_type::name>::type, void>){\
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
		if constexpr (std::is_same_v<godot::detail::ReturnType<&self::value_type::name>::type, void>)\
			temp.name(std::forward<Args>(args)...);\
		else return temp.name(std::forward<Args>(args)...);\
	}

// Helper that creates the different permutations of a property that copies if there is a getter or a setter for the parent property
// #define GODOT_PROPERTY_WRAPPED_PROPERTY_NO_GET_SET(type, name, self) Property<type, &self::get_##name, &self::set_##name> name() requires getsetable<self> { return this; }\
// 	Property<type, &self::get_##name> name() requires (!getsetable<self> && getable<self>) { return this; }\
// 	Property<type, nullptr, &self::set_##name> name() requires (!getsetable<self> && setable<self>) { return this; }\
//     const Property<type, &Self::get_##name, &Self::set_##name> name() const requires getsetable<Self> { return this; }\
// 	const Property<type, &Self::get_##name> name() const requires (!getsetable<Self> && getable<Self>) { return this; }
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

} // namespace godot

#endif // GODOT_PROPERTIES_HPP