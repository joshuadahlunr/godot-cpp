/**************************************************************************/
/*  basis.hpp                                                             */
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

#ifndef GODOT_BASIS_HPP
#define GODOT_BASIS_HPP

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/property_wrappers.hpp>
#include <godot_cpp/variant/quaternion.hpp>
#include <godot_cpp/variant/vector3.hpp>

namespace godot {

struct _NO_DISCARD_ Basis {
	Vector3 rows[3] = {
		Vector3(1, 0, 0),
		Vector3(0, 1, 0),
		Vector3(0, 0, 1)
	};

	_FORCE_INLINE_ const Vector3 &operator[](int axis) const {
		return rows[axis];
	}
	_FORCE_INLINE_ Vector3 &operator[](int axis) {
		return rows[axis];
	}

	Vector3 get_x() const { return rows[0]; }
	Vector3 set_x(Vector3 value) { return rows[0] = value; }
	godot::Property<Vector3, &Basis::get_x, &Basis::set_x> x() { return this; }

	Vector3 get_y() const { return rows[1]; }
	Vector3 set_y(Vector3 value) { return rows[1] = value; }
	godot::Property<Vector3, &Basis::get_y, &Basis::set_y> y() { return this; }

	Vector3 get_z() const { return rows[2]; }
	Vector3 set_z(Vector3 value) { return rows[2] = value; }
	godot::Property<Vector3, &Basis::get_z, &Basis::set_z> z() { return this; }


	void invert();
	void transpose();

	Basis inverse() const;
	Basis transposed() const;

	_FORCE_INLINE_ real_t determinant() const;

	void from_z(const Vector3 &p_z);

	void rotate(const Vector3 &p_axis, Math::Radian p_angle);
	Basis rotated(const Vector3 &p_axis, Math::Radian p_angle) const;

	void rotate_local(const Vector3 &p_axis, Math::Radian p_angle);
	Basis rotated_local(const Vector3 &p_axis, Math::Radian p_angle) const;

	void rotate(const Vector3 &p_euler, EulerOrder p_order = EULER_ORDER_YXZ);
	Basis rotated(const Vector3 &p_euler, EulerOrder p_order = EULER_ORDER_YXZ) const;

	void rotate(const Quaternion &p_quaternion);
	Basis rotated(const Quaternion &p_quaternion) const;

	Vector3 get_euler_normalized(EulerOrder p_order = EULER_ORDER_YXZ) const;
	void get_rotation_axis_angle(Vector3 &p_axis, Math::Radian &p_angle) const;
	void get_rotation_axis_angle_local(Vector3 &p_axis, Math::Radian &p_angle) const;
	Quaternion get_rotation_quaternion() const;

	void rotate_to_align(Vector3 p_start_direction, Vector3 p_end_direction);

	Vector3 rotref_posscale_decomposition(Basis &rotref) const;

	Vector3 get_euler(EulerOrder p_order = EULER_ORDER_YXZ) const;
	void set_euler(const Vector3 &p_euler, EulerOrder p_order = EULER_ORDER_YXZ);
	static Basis from_euler(const Vector3 &p_euler, EulerOrder p_order = EULER_ORDER_YXZ) {
		Basis b;
		b.set_euler(p_euler, p_order);
		return b;
	}

	Quaternion get_quaternion() const;
	void set_quaternion(const Quaternion &p_quaternion);

	void get_axis_angle(Vector3 &r_axis, Math::Radian &r_angle) const;
	void set_axis_angle(const Vector3 &p_axis, Math::Radian p_angle);

	void scale(const Vector3 &p_scale);
	Basis scaled(const Vector3 &p_scale) const;

	void scale_local(const Vector3 &p_scale);
	Basis scaled_local(const Vector3 &p_scale) const;

	void scale_orthogonal(const Vector3 &p_scale);
	Basis scaled_orthogonal(const Vector3 &p_scale) const;

	void make_scale_uniform();
	float get_uniform_scale() const;

	Vector3 get_scale() const;
	Vector3 get_scale_abs() const;
	Vector3 get_scale_local() const;

	void set_axis_angle_scale(const Vector3 &p_axis, real_t p_angle, const Vector3 &p_scale);
	void set_euler_scale(const Vector3 &p_euler, const Vector3 &p_scale, EulerOrder p_order = EULER_ORDER_YXZ);
	void set_quaternion_scale(const Quaternion &p_quaternion, const Vector3 &p_scale);

	// transposed dot products
	_FORCE_INLINE_ real_t tdotx(const Vector3 &v) const {
		return rows[0][0] * v[0] + rows[1][0] * v[1] + rows[2][0] * v[2];
	}
	_FORCE_INLINE_ real_t tdoty(const Vector3 &v) const {
		return rows[0][1] * v[0] + rows[1][1] * v[1] + rows[2][1] * v[2];
	}
	_FORCE_INLINE_ real_t tdotz(const Vector3 &v) const {
		return rows[0][2] * v[0] + rows[1][2] * v[1] + rows[2][2] * v[2];
	}

	bool is_equal_approx(const Basis &p_basis) const;

	bool operator==(const Basis &p_matrix) const;
	bool operator!=(const Basis &p_matrix) const;

	_FORCE_INLINE_ Vector3 xform(const Vector3 &p_vector) const;
	_FORCE_INLINE_ Vector3 xform_inv(const Vector3 &p_vector) const;
	_FORCE_INLINE_ void operator*=(const Basis &p_matrix);
	_FORCE_INLINE_ Basis operator*(const Basis &p_matrix) const;
	_FORCE_INLINE_ void operator+=(const Basis &p_matrix);
	_FORCE_INLINE_ Basis operator+(const Basis &p_matrix) const;
	_FORCE_INLINE_ void operator-=(const Basis &p_matrix);
	_FORCE_INLINE_ Basis operator-(const Basis &p_matrix) const;
	_FORCE_INLINE_ void operator*=(const real_t p_val);
	_FORCE_INLINE_ Basis operator*(const real_t p_val) const;

	bool is_orthogonal() const;
	bool is_diagonal() const;
	bool is_rotation() const;

	Basis lerp(const Basis &p_to, const real_t &p_weight) const;
	Basis slerp(const Basis &p_to, const real_t &p_weight) const;
	void rotate_sh(real_t *p_values);

	operator String() const;

	/* create / set */

	_FORCE_INLINE_ void set(real_t xx, real_t xy, real_t xz, real_t yx, real_t yy, real_t yz, real_t zx, real_t zy, real_t zz) {
		rows[0][0] = xx;
		rows[0][1] = xy;
		rows[0][2] = xz;
		rows[1][0] = yx;
		rows[1][1] = yy;
		rows[1][2] = yz;
		rows[2][0] = zx;
		rows[2][1] = zy;
		rows[2][2] = zz;
	}
	_FORCE_INLINE_ void set_columns(const Vector3 &p_x, const Vector3 &p_y, const Vector3 &p_z) {
		set_column(0, p_x);
		set_column(1, p_y);
		set_column(2, p_z);
	}

	_FORCE_INLINE_ Vector3 get_column(int p_index) const {
		// Get actual basis axis column (we store transposed as rows for performance).
		return Vector3(rows[0][p_index], rows[1][p_index], rows[2][p_index]);
	}

	_FORCE_INLINE_ void set_column(int p_index, const Vector3 &p_value) {
		// Set actual basis axis column (we store transposed as rows for performance).
		rows[0][p_index] = p_value.x;
		rows[1][p_index] = p_value.y;
		rows[2][p_index] = p_value.z;
	}

	_FORCE_INLINE_ Vector3 get_main_diagonal() const {
		return Vector3(rows[0][0], rows[1][1], rows[2][2]);
	}

	_FORCE_INLINE_ void set_zero() {
		rows[0].zero();
		rows[1].zero();
		rows[2].zero();
	}

	_FORCE_INLINE_ Basis transpose_xform(const Basis &m) const {
		return Basis(
				rows[0].x * m[0].x + rows[1].x * m[1].x + rows[2].x * m[2].x,
				rows[0].x * m[0].y + rows[1].x * m[1].y + rows[2].x * m[2].y,
				rows[0].x * m[0].z + rows[1].x * m[1].z + rows[2].x * m[2].z,
				rows[0].y * m[0].x + rows[1].y * m[1].x + rows[2].y * m[2].x,
				rows[0].y * m[0].y + rows[1].y * m[1].y + rows[2].y * m[2].y,
				rows[0].y * m[0].z + rows[1].y * m[1].z + rows[2].y * m[2].z,
				rows[0].z * m[0].x + rows[1].z * m[1].x + rows[2].z * m[2].x,
				rows[0].z * m[0].y + rows[1].z * m[1].y + rows[2].z * m[2].y,
				rows[0].z * m[0].z + rows[1].z * m[1].z + rows[2].z * m[2].z);
	}
	Basis(real_t xx, real_t xy, real_t xz, real_t yx, real_t yy, real_t yz, real_t zx, real_t zy, real_t zz) {
		set(xx, xy, xz, yx, yy, yz, zx, zy, zz);
	}

	void orthonormalize();
	Basis orthonormalized() const;

	void orthogonalize();
	Basis orthogonalized() const;

#ifdef MATH_CHECKS
	bool is_symmetric() const;
#endif
	Basis diagonalize();

	operator Quaternion() const { return get_quaternion(); }

	static Basis looking_at(const Vector3 &p_target, const Vector3 &p_up = Vector3(0, 1, 0));

	Basis(const Quaternion &p_quaternion) { set_quaternion(p_quaternion); }
	Basis(const Quaternion &p_quaternion, const Vector3 &p_scale) { set_quaternion_scale(p_quaternion, p_scale); }

	Basis(const Vector3 &p_axis, real_t p_angle) { set_axis_angle(p_axis, p_angle); }
	Basis(const Vector3 &p_axis, real_t p_angle, const Vector3 &p_scale) { set_axis_angle_scale(p_axis, p_angle, p_scale); }
	static Basis from_scale(const Vector3 &p_scale);

	_FORCE_INLINE_ Basis(const Vector3 &p_x_axis, const Vector3 &p_y_axis, const Vector3 &p_z_axis) {
		set_columns(p_x_axis, p_y_axis, p_z_axis);
	}

	_FORCE_INLINE_ Basis() {}

private:
	// Helper method.
	void _set_diagonal(const Vector3 &p_diag);
};

_FORCE_INLINE_ void Basis::operator*=(const Basis &p_matrix) {
	set(
			p_matrix.tdotx(rows[0]), p_matrix.tdoty(rows[0]), p_matrix.tdotz(rows[0]),
			p_matrix.tdotx(rows[1]), p_matrix.tdoty(rows[1]), p_matrix.tdotz(rows[1]),
			p_matrix.tdotx(rows[2]), p_matrix.tdoty(rows[2]), p_matrix.tdotz(rows[2]));
}

_FORCE_INLINE_ Basis Basis::operator*(const Basis &p_matrix) const {
	return Basis(
			p_matrix.tdotx(rows[0]), p_matrix.tdoty(rows[0]), p_matrix.tdotz(rows[0]),
			p_matrix.tdotx(rows[1]), p_matrix.tdoty(rows[1]), p_matrix.tdotz(rows[1]),
			p_matrix.tdotx(rows[2]), p_matrix.tdoty(rows[2]), p_matrix.tdotz(rows[2]));
}

_FORCE_INLINE_ void Basis::operator+=(const Basis &p_matrix) {
	rows[0] += p_matrix.rows[0];
	rows[1] += p_matrix.rows[1];
	rows[2] += p_matrix.rows[2];
}

_FORCE_INLINE_ Basis Basis::operator+(const Basis &p_matrix) const {
	Basis ret(*this);
	ret += p_matrix;
	return ret;
}

_FORCE_INLINE_ void Basis::operator-=(const Basis &p_matrix) {
	rows[0] -= p_matrix.rows[0];
	rows[1] -= p_matrix.rows[1];
	rows[2] -= p_matrix.rows[2];
}

_FORCE_INLINE_ Basis Basis::operator-(const Basis &p_matrix) const {
	Basis ret(*this);
	ret -= p_matrix;
	return ret;
}

_FORCE_INLINE_ void Basis::operator*=(const real_t p_val) {
	rows[0] *= p_val;
	rows[1] *= p_val;
	rows[2] *= p_val;
}

_FORCE_INLINE_ Basis Basis::operator*(const real_t p_val) const {
	Basis ret(*this);
	ret *= p_val;
	return ret;
}

Vector3 Basis::xform(const Vector3 &p_vector) const {
	return Vector3(
			rows[0].dot(p_vector),
			rows[1].dot(p_vector),
			rows[2].dot(p_vector));
}

Vector3 Basis::xform_inv(const Vector3 &p_vector) const {
	return Vector3(
			(rows[0][0] * p_vector.x) + (rows[1][0] * p_vector.y) + (rows[2][0] * p_vector.z),
			(rows[0][1] * p_vector.x) + (rows[1][1] * p_vector.y) + (rows[2][1] * p_vector.z),
			(rows[0][2] * p_vector.x) + (rows[1][2] * p_vector.y) + (rows[2][2] * p_vector.z));
}

real_t Basis::determinant() const {
	return rows[0][0] * (rows[1][1] * rows[2][2] - rows[2][1] * rows[1][2]) -
			rows[1][0] * (rows[0][1] * rows[2][2] - rows[2][1] * rows[0][2]) +
			rows[2][0] * (rows[0][1] * rows[1][2] - rows[1][1] * rows[0][2]);
}

template <auto Getter, auto Setter> PROPERTY_TEMPLATE_CONSTRAINT(Getter, Setter)
class Property<Basis, Getter, Setter> : public PropertyOperations<Property<Basis, Getter, Setter>> {
    using T = Basis;
    using Self = Property<Basis, Getter, Setter>;
public:
	PROPERTY_CORE(Getter, Setter)

	Vector3* get_rows() const { return get().rows; }
	Vector3* set_rows(Vector3* value) {
		auto temp = get();
		memcpy(value, temp.rows, 3);
		set(temp);
		return temp.rows;
	}
	GODOT_PROPERTY_WRAPPED_PROPERTY_NO_GET_SET(Vector3*, rows, Self);

	Vector3 get_x() const { return get_rows()[0]; }
	Vector3 get_y() const { return get_rows()[1]; }
	Vector3 get_z() const { return get_rows()[2]; }

	Vector3 set_x(Vector3 value) {
		auto temp = get_rows();
		temp[0] = value;
		set_rows(temp);
		return temp[0];
	}
	Vector3 set_y(Vector3 value) {
		auto temp = get_rows();
		temp[1] = value;
		set_rows(temp);
		return temp[1];
	}
	Vector3 set_z(Vector3 value) {
		auto temp = get_rows();
		temp[2] = value;
		set_rows(temp);
		return temp[2];
	}

	GODOT_PROPERTY_WRAPPED_PROPERTY_NO_GET_SET(Vector3, x, Self);
	GODOT_PROPERTY_WRAPPED_PROPERTY_NO_GET_SET(Vector3, y, Self);
	GODOT_PROPERTY_WRAPPED_PROPERTY_NO_GET_SET(Vector3, z, Self);

	GODOT_PROPERTY_WRAPPED_FUNCTION(invert, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(transpose, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(inverse, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(transposed, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(determinant, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(from_z, Self)
	// Have to be specified manually because overload sets don't go through the template mechanisms properly...
	template<typename... Args> requires (getsetable<Self>) void rotate(Args... args) { auto temp = get(); temp.rotate(std::forward<Args>(args)...); set(temp); }
	template<typename... Args> requires (!getsetable<Self> && getable<Self>) void rotate(Args... args) const { const auto temp = get(); temp.rotate(std::forward<Args>(args)...); }
	template<typename... Args> requires (getsetable<Self>) auto rotated(Args... args) { auto temp = get(); auto ret = temp.rotated(std::forward<Args>(args)...); set(temp); return ret; }
	template<typename... Args> requires (!getsetable<Self> && getable<Self>) auto rotated(Args... args) const { const auto temp = get(); auto ret = temp.rotated(std::forward<Args>(args)...); return ret; }
	GODOT_PROPERTY_WRAPPED_FUNCTION(rotate_local, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(rotated_local, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(get_euler_normalized, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(get_rotation_axis_angle, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(get_rotation_axis_angle_local, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(get_rotation_quaternion, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(rotate_to_align, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(rotref_posscale_decomposition, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(get_euler, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(set_euler, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(from_euler, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(get_quaternion, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(set_quaternion, Self)
	GODOT_PROPERTY_WRAPPED_PROPERTY_NO_GET_SET(Quaternion, quaternion, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(get_axis_angle, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(set_axis_angle, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(scale, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(scaled, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(scale_local, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(scaled_local, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(scale_orthogonal, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(scaled_orthogonal, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(make_scale_uniform, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(get_uniform_scale, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(get_scale, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(get_scale_abs, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(get_scale_local, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(set_axis_angle_scale, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(set_euler_scale, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(set_quaternion_scale, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(tdotx, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(tdoty, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(tdotz, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(is_equal_approx, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(xform, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(xform_inv, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(is_orthogonal, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(is_diagonal, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(is_rotation, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(lerp, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(slerp, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(rotate_sh, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(set, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(set_columns, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(get_column, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(set_column, Self)
	GODOT_PROPERTY_WRAPPED_PROPERTY_NO_GET_SET(Vector3, column, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(get_main_diagonal, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(set_zero, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(transpose_xform, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(orthonormalize, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(orthonormalized, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(orthogonalize, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(orthogonalized, Self)
#ifdef MATH_CHECKS
	GODOT_PROPERTY_WRAPPED_FUNCTION(is_symmetric, Self)
#endif
	GODOT_PROPERTY_WRAPPED_FUNCTION(diagonalize, Self)
	GODOT_PROPERTY_WRAPPED_FUNCTION(looking_at, Self)
};

} // namespace godot

#endif // GODOT_BASIS_HPP
