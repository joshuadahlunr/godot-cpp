/**************************************************************************/
/*  plane.hpp                                                             */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef GODOT_PLANE_HPP
#define GODOT_PLANE_HPP

#include <godot_cpp/classes/properties.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/variant/vector3.hpp>

namespace godot {

class Variant;

struct _NO_DISCARD_ Plane {
	Vector3 normal;
	real_t d = 0;

	void set_normal(const Vector3 &p_normal);
	_FORCE_INLINE_ Vector3 get_normal() const { return normal; }

	float get_x() const { return normal.x; }
	float set_x(float value) { return normal.x = value; }
	godot::Property<float, &Plane::get_x, &Plane::set_x> x() { return this; }

	float get_y() const { return normal.y; }
	float set_y(float value) { return normal.y = value; }
	godot::Property<float, &Plane::get_y, &Plane::set_y> y() { return this; }

	float get_z() const { return normal.z; }
	float set_z(float value) { return normal.z = value; }
	godot::Property<float, &Plane::get_z, &Plane::set_z> z() { return this; }


	void normalize();
	Plane normalized() const;

	/* Plane-Point operations */

	_FORCE_INLINE_ Vector3 center() const { return normal * d; }
	Vector3 get_any_perpendicular_normal() const;

	_FORCE_INLINE_ bool is_point_over(const Vector3 &p_point) const; ///< Point is over plane
	_FORCE_INLINE_ real_t distance_to(const Vector3 &p_point) const;
	_FORCE_INLINE_ bool has_point(const Vector3 &p_point, real_t p_tolerance = CMP_EPSILON) const;

	/* intersections */

	bool intersect_3(const Plane &p_plane1, const Plane &p_plane2, Vector3 *r_result = nullptr) const;
	bool intersects_ray(const Vector3 &p_from, const Vector3 &p_dir, Vector3 *p_intersection) const;
	bool intersects_segment(const Vector3 &p_begin, const Vector3 &p_end, Vector3 *p_intersection) const;

	// For Variant bindings.
	Variant intersect_3_bind(const Plane &p_plane1, const Plane &p_plane2) const;
	Variant intersects_ray_bind(const Vector3 &p_from, const Vector3 &p_dir) const;
	Variant intersects_segment_bind(const Vector3 &p_begin, const Vector3 &p_end) const;

	_FORCE_INLINE_ Vector3 project(const Vector3 &p_point) const {
		return p_point - normal * distance_to(p_point);
	}

	/* misc */

	Plane operator-() const { return Plane(-normal, -d); }
	bool is_equal_approx(const Plane &p_plane) const;
	bool is_equal_approx_any_side(const Plane &p_plane) const;

	_FORCE_INLINE_ bool operator==(const Plane &p_plane) const;
	_FORCE_INLINE_ bool operator!=(const Plane &p_plane) const;
	operator String() const;

	_FORCE_INLINE_ Plane() {}
	_FORCE_INLINE_ Plane(real_t p_a, real_t p_b, real_t p_c, real_t p_d) :
			normal(p_a, p_b, p_c),
			d(p_d) {}

	_FORCE_INLINE_ Plane(const Vector3 &p_normal, real_t p_d = 0.0);
	_FORCE_INLINE_ Plane(const Vector3 &p_normal, const Vector3 &p_point);
	_FORCE_INLINE_ Plane(const Vector3 &p_point1, const Vector3 &p_point2, const Vector3 &p_point3, ClockDirection p_dir = CLOCKWISE);
};

bool Plane::is_point_over(const Vector3 &p_point) const {
	return (normal.dot(p_point) > d);
}

real_t Plane::distance_to(const Vector3 &p_point) const {
	return (normal.dot(p_point) - d);
}

bool Plane::has_point(const Vector3 &p_point, real_t p_tolerance) const {
	real_t dist = normal.dot(p_point) - d;
	dist = Math::abs(dist);
	return (dist <= p_tolerance);
}

Plane::Plane(const Vector3 &p_normal, real_t p_d) :
		normal(p_normal),
		d(p_d) {
}

Plane::Plane(const Vector3 &p_normal, const Vector3 &p_point) :
		normal(p_normal),
		d(p_normal.dot(p_point)) {
}

Plane::Plane(const Vector3 &p_point1, const Vector3 &p_point2, const Vector3 &p_point3, ClockDirection p_dir) {
	if (p_dir == CLOCKWISE) {
		normal = (p_point1 - p_point3).cross(p_point1 - p_point2);
	} else {
		normal = (p_point1 - p_point2).cross(p_point1 - p_point3);
	}

	normal.normalize();
	d = normal.dot(p_point1);
}

bool Plane::operator==(const Plane &p_plane) const {
	return normal == p_plane.normal && d == p_plane.d;
}

bool Plane::operator!=(const Plane &p_plane) const {
	return normal != p_plane.normal || d != p_plane.d;
}

template <auto Getter, auto Setter> PROPERTY_TEMPLATE_CONSTRAINT(Getter, Setter)
class Property<Plane, Getter, Setter> : public PropertyOperations<Property<Plane, Getter, Setter>> {
    using T = Plane;
    using Self = Property<Plane, Getter, Setter>;
public:
	PROPERTY_CORE(Getter, Setter)

	float get_x() const { return get_normal().x; }
	float get_y() const { return get_normal().y; }
	float get_z() const { return get_normal().z; }

	float set_x(float value) {
		auto temp = get_normal();
		temp.x = value;
		set_normal(temp);
		return temp.x;
	}
	float set_y(float value) {
		auto temp = get_normal();
		temp.y = value;
		set_normal(temp);
		return temp.y;
	}
	float set_z(float value) {
		auto temp = get_normal();
		temp.z = value;
		set_normal(temp);
		return temp.z;
	}

	GODOT_PROPERTY_WRAPPED_PROPERTY_NO_GET_SET(float, x, Self);
	GODOT_PROPERTY_WRAPPED_PROPERTY_NO_GET_SET(float, y, Self);
	GODOT_PROPERTY_WRAPPED_PROPERTY_NO_GET_SET(float, z, Self);

	GODOT_PROPERTY_WRAPPED_PROPERTY(Vector3, normal, Self)
	GODOT_PROPERTY_WRAPPED_PROPERTY(real_t, d, Self)

	GODOT_PROPERTY_WRAPPED_FUNCTION(normalize, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(normalized, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(center, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(get_any_perpendicular_normal, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(is_point_over, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(distance_to, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(has_point, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(intersect_3, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(intersects_ray, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(intersects_segment, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(intersect_3_bind, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(intersects_ray_bind, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(intersects_segment_bind, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(project, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(is_equal_approx, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(is_equal_approx_any_side, Self)

};

} // namespace godot

#endif // GODOT_PLANE_HPP
