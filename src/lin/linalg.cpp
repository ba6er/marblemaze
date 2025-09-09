#include <lin/linalg.hpp>
#include <util.hpp>
#include <cmath>

using namespace lin;

Vec2 Vec2::operator-() const {
	Vec2 result = {-x, -y};
	if (std::isnan(result.x) || std::isnan(result.y)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

Vec2 Vec2::operator+(Vec2 v) const {
	Vec2 result = {x + v.x, y + v.y};
	if (std::isnan(result.x) || std::isnan(result.y)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

Vec2 Vec2::operator-(Vec2 v) const {
	Vec2 result = {x - v.x, y - v.y};
	if (std::isnan(result.x) || std::isnan(result.y)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

Vec2 Vec2::operator*(float n) const {
	Vec2 result = {x * n, y * n};
	if (std::isnan(result.x) || std::isnan(result.y)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

Vec2 Vec2::operator/(float n) const {
	Vec2 result = {x / n, y / n};
	if (std::isnan(result.x) || std::isnan(result.y)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

Vec4 Vec4::operator-() const {
	Vec4 result = {-x, -y, -z, -w};
	if (std::isnan(result.x) || std::isnan(result.y) || std::isnan(result.z) || std::isnan(result.w)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

Vec4 Vec4::operator+(Vec4 v) const {
	Vec4 result = {x + v.x, y + v.y, z + v.z, w + v.w};
	if (std::isnan(result.x) || std::isnan(result.y) || std::isnan(result.z) || std::isnan(result.w)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

Vec4 Vec4::operator-(Vec4 v) const {
	Vec4 result = {x - v.x, y - v.y, z - v.z, w - v.w};
	if (std::isnan(result.x) || std::isnan(result.y) || std::isnan(result.z) || std::isnan(result.w)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

Vec4 Vec4::operator*(float n) const {
	Vec4 result = {x * n, y * n, z * n, w * n};
	if (std::isnan(result.x) || std::isnan(result.y) || std::isnan(result.z) || std::isnan(result.w)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

Vec4 Vec4::operator/(float n) const {
	Vec4 result = {x / n, y / n, z / n, w / n};
	if (std::isnan(result.x) || std::isnan(result.y) || std::isnan(result.z) || std::isnan(result.w)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

Vec3 Vec3::operator-() const {
	Vec3 result = {-x, -y, -z};
	if (std::isnan(result.x) || std::isnan(result.y) || std::isnan(result.z)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

Vec3 Vec3::operator+(Vec3 v) const {
	Vec3 result = {x + v.x, y + v.y, z + v.z};
	if (std::isnan(result.x) || std::isnan(result.y) || std::isnan(result.z)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

Vec3 Vec3::operator-(Vec3 v) const {
	Vec3 result = {x - v.x, y - v.y, z - v.z};
	if (std::isnan(result.x) || std::isnan(result.y) || std::isnan(result.z)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

Vec3 Vec3::operator*(float n) const {
	Vec3 result = {x * n, y * n, z * n};
	if (std::isnan(result.x) || std::isnan(result.y) || std::isnan(result.z)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

Vec3 Vec3::operator/(float n) const {
	Vec3 result = {x / n, y / n, z / n};
	if (std::isnan(result.x) || std::isnan(result.y) || std::isnan(result.z)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

void Vec3::operator+=(Vec3 v) {
	x += v.x;
	y += v.y;
	z += v.z;
	if (std::isnan(x) || std::isnan(y) || std::isnan(z)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
}

void Vec3::operator-=(Vec3 v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	if (std::isnan(x) || std::isnan(y) || std::isnan(z)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
}

void Vec3::operator*=(float n) {
	x *= n;
	y *= n;
	z *= n;
	if (std::isnan(x) || std::isnan(y) || std::isnan(z)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
}

void Vec3::operator/=(float n) {
	x /= n;
	y /= n;
	z /= n;
	if (std::isnan(x) || std::isnan(y) || std::isnan(z)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
}

Vec3 Vec3::normalize() const {
	float len = length();
	return (*this) / len;
}

Vec3 Vec3::cross(Vec3 v) const {
	Vec3 result = {
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x};
	if (std::isnan(result.x) || std::isnan(result.y) || std::isnan(result.z)) {
		DEBUG_WARNING("Vector values are NaN!");
	}
	return result;
}

float Vec3::dot(Vec3 v) const {
	return (x * v.x) + (y * v.y) + (z * v.z);
}

float Vec3::length() const {
	return std::sqrtf(x * x + y * y + z * z);
}

Mat4 Mat4::Identity() {
	return {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1};
}

Mat4 Mat4::Scale(float x, float y, float z) {
	return {
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1};
}

Mat4 Mat4::Scale(Vec3 scalar) {
	return Scale(scalar.x, scalar.y, scalar.z);
}

Mat4 Mat4::Rotate(float a, float nx, float ny, float nz) {
	float sa = std::sinf(a);
	float ca = std::cosf(a);
	float na = 1 - ca;
	return {
		nx * nx * na + ca,      ny * nx * na + nz * sa, nz * nx * na - ny * sa, 0.0f,
		nx * ny * na - nz * sa, ny * ny * na + ca,      nz * ny * na + nx * sa, 0.0f,
		nx * nz * na + ny * sa, ny * nz * na - nx * sa, nz * nz * na + ca,      0.0f,
		0.0f,                   0.0f,                   0.0f,                   1.0f};
}

Mat4 Mat4::Rotate(float a, Vec3 axis) {
	return Rotate(a, axis.x, axis.y, axis.z);
}

Mat4 Mat4::Translate(float x, float y, float z) {
	return {
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1};
}

Mat4 Mat4::Translate(Vec3 translation) {
	return Translate(translation.x, translation.y, translation.z);
}

Mat4 Mat4::LookAt(Vec3 eye, Vec3 target, Vec3 up) {
	Mat4 m = Identity();

	Vec3 vz = (eye - target).normalize();
	Vec3 vx = up.cross(vz).normalize();
	Vec3 vy = vz.cross(vx).normalize();

	m.x1 = vx.x;
	m.y1 = vy.x;
	m.z1 = vz.x;

	m.x2 = vx.y;
	m.y2 = vy.y;
	m.z2 = vz.y;

	m.x3 = vx.z;
	m.y3 = vy.z;
	m.z3 = vz.z;

	m.x4 = -vx.dot(eye);
	m.y4 = -vy.dot(eye);
	m.z4 = -vz.dot(eye);

	return m;
}

Mat4 Mat4::Project2d(float left, float right, float bottom, float top, float near, float far) {
	Mat4 m = Identity();

	m.x1 = 2 / (right - left);
	m.y2 = 2 / (bottom - top);
	m.z3 = 2 / (near - far);

	m.x4 = (left + right) / (left - right);
	m.y4 = (bottom + top) / (top - bottom);
	m.z4 = (far  + near)  / (far  - near);

	return m;
}

Mat4 Mat4::Project3d(float fov, float aspect, float near, float far) {
	Mat4 m = Identity();

	float top = near * std::tanf(fov / 2);
	float right = top * aspect;

	m.x1 = near / right;
	m.y2 = near / top;
	m.z3 = (far + near) / (near - far);
	m.w3 = -1.0f;
	m.z4 = 2 * near * far / (near - far);

	return m;
}

Mat4 Mat4::operator+(const Mat4& m) const {
	return {
		x1 + m.x1, x2 + m.x2, x3 + m.x3, x4 + m.x4,
		y1 + m.x1, y2 + m.x2, y3 + m.x3, y4 + m.x4,
		z1 + m.z1, z2 + m.z2, z3 + m.z3, z4 + m.z4,
		w1 + m.w1, w2 + m.w2, w3 + m.w3, w4 + m.w4};
}

Mat4 Mat4::operator-(const Mat4& m) const {
	return {
		x1 - m.x1, x2 - m.x2, x3 - m.x3, x4 - m.x4,
		y1 - m.x1, y2 - m.x2, y3 - m.x3, y4 - m.x4,
		z1 - m.z1, z2 - m.z2, z3 - m.z3, z4 - m.z4,
		w1 - m.w1, w2 - m.w2, w3 - m.w3, w4 - m.w4};
}

Mat4 Mat4::operator*(const Mat4& m) const {
	return {
		(x1 * m.x1) + (x2 * m.y1) + (x3 * m.z1) + (x4 * m.w1),
		(x1 * m.x2) + (x2 * m.y2) + (x3 * m.z2) + (x4 * m.w2),
		(x1 * m.x3) + (x2 * m.y3) + (x3 * m.z3) + (x4 * m.w3),
		(x1 * m.x4) + (x2 * m.y4) + (x3 * m.z4) + (x4 * m.w4),
		(y1 * m.x1) + (y2 * m.y1) + (y3 * m.z1) + (y4 * m.w1),
		(y1 * m.x2) + (y2 * m.y2) + (y3 * m.z2) + (y4 * m.w2),
		(y1 * m.x3) + (y2 * m.y3) + (y3 * m.z3) + (y4 * m.w3),
		(y1 * m.x4) + (y2 * m.y4) + (y3 * m.z4) + (y4 * m.w4),
		(z1 * m.x1) + (z2 * m.y1) + (z3 * m.z1) + (z4 * m.w1),
		(z1 * m.x2) + (z2 * m.y2) + (z3 * m.z2) + (z4 * m.w2),
		(z1 * m.x3) + (z2 * m.y3) + (z3 * m.z3) + (z4 * m.w3),
		(z1 * m.x4) + (z2 * m.y4) + (z3 * m.z4) + (z4 * m.w4),
		(w1 * m.x1) + (w2 * m.y1) + (w3 * m.z1) + (w4 * m.w1),
		(w1 * m.x2) + (w2 * m.y2) + (w3 * m.z2) + (w4 * m.w2),
		(w1 * m.x3) + (w2 * m.y3) + (w3 * m.z3) + (w4 * m.w3),
		(w1 * m.x4) + (w2 * m.y4) + (w3 * m.z4) + (w4 * m.w4)};
}

Mat4 Mat4::transpose() const {
	return {
		x1, y1, z1, w1,
		x2, y2, z2, w2,
		x3, y3, z3, w3,
		x4, y4, z4, w4};
}

Mat4 Mat4::inverse() const {
	Mat4 inv;

	inv.x1 =
		y2 * z3 * w4 -
		y2 * z4 * w3 -
		z2 * y3 * w4 +
		z2 * y4 * w3 +
		w2 * y3 * z4 -
		w2 * y4 * z3;

	inv.y1 =
		y1 * z4 * w3 -
		y1 * z3 * w4 +
		z1 * y3 * w4 -
		z1 * y4 * w3 -
		w1 * y3 * z4 +
		w1 * y4 * z3;

	inv.z1 =
		y1 * z2 * w4 -
		y1 * z4 * w2 -
		z1 * y2 * w4 +
		z1 * y4 * w2 +
		w1 * y2 * z4 -
		w1 * y4 * z2;

	inv.w1 =
		y1 * z3 * w2 -
		y1 * z2 * w3 +
		z1 * y2 * w3 -
		z1 * y3 * w2 -
		w1 * y2 * z3 +
		w1 * y3 * z2;

	inv.x2 =
		x2 * z4 * w3 -
		x2 * z3 * w4 +
		z2 * x3 * w4 -
		z2 * x4 * w3 -
		w2 * x3 * z4 +
		w2 * x4 * z3;

	inv.y2 =
		x1 * z3 * w4 -
		x1 * z4 * w3 -
		z1 * x3 * w4 +
		z1 * x4 * w3 +
		w1 * x3 * z4 -
		w1 * x4 * z3;

	inv.z2 =
		x1 * z4 * w2 -
		x1 * z2 * w4 +
		z1 * x2 * w4 -
		z1 * x4 * w2 -
		w1 * x2 * z4 +
		w1 * x4 * z2;

	inv.w2 =
		x1 * z2 * w3 -
		x1 * z3 * w2 -
		z1 * x2 * w3 +
		z1 * x3 * w2 +
		w1 * x2 * z3 -
		w1 * x3 * z2;

	inv.x3 =
		x2 * y3 * w4 -
		x2 * y4 * w3 -
		y2 * x3 * w4 +
		y2 * x4 * w3 +
		w2 * x3 * y4 -
		w2 * x4 * y3;

	inv.y3 =
		x1 * y4 * w3 -
		x1 * y3 * w4 +
		y1 * x3 * w4 -
		y1 * x4 * w3 -
		w1 * x3 * y4 +
		w1 * x4 * y3;

	inv.z3 =
		x1 * y2 * w4 -
		x1 * y4 * w2 -
		y1 * x2 * w4 +
		y1 * x4 * w2 +
		w1 * x2 * y4 -
		w1 * x4 * y2;

	inv.w3 =
		x1 * y3 * w2 -
		x1 * y2 * w3 +
		y1 * x2 * w3 -
		y1 * x3 * w2 -
		w1 * x2 * y3 +
		w1 * x3 * y2;

	inv.x4 =
		x2 * y4 * z3 -
		x2 * y3 * z4 +
		y2 * x3 * z4 -
		y2 * x4 * z3 -
		z2 * x3 * y4 +
		z2 * x4 * y3;

	inv.y4 =
		x1 * y3 * z4 -
		x1 * y4 * z3 -
		y1 * x3 * z4 +
		y1 * x4 * z3 +
		z1 * x3 * y4 -
		z1 * x4 * y3;

	inv.z4 =
		x1 * y4 * z2 -
		x1 * y2 * z4 +
		y1 * x2 * z4 -
		y1 * x4 * z2 -
		z1 * x2 * y4 +
		z1 * x4 * y2;

	inv.w4 =
		x1 * y2 * z3 -
		x1 * y3 * z2 -
		y1 * x2 * z3 +
		y1 * x3 * z2 +
		z1 * x2 * y3 -
		z1 * x3 * y2;

	float det = x1 * inv.x1 + x2 * inv.y1 + x3 * inv.z1 + x4 * inv.w1;

	if (std::abs(det) < 1e-6f) {
		DEBUG_WARNING("Matrix determinant is 0, inverse cannot be computed");
		return Identity();
	}
	inv.x1 /= det; inv.x2 /= det; inv.x3 /= det; inv.x4 /= det;
	inv.y1 /= det; inv.y2 /= det; inv.y3 /= det; inv.y4 /= det;
	inv.z1 /= det; inv.z2 /= det; inv.z3 /= det; inv.z4 /= det;
	inv.w1 /= det; inv.w2 /= det; inv.w3 /= det; inv.w4 /= det;
	return inv;
}

Vec3 Mat4::operator*(Vec3 v) const {
	Vec4 v4 = {v.x, v.y, v.z, 1.0f};
	Vec4 result = (*this) * v4;
	return {result.x, result.y, result.z};
}

Vec4 Mat4::operator*(Vec4 v) const {
	return {
		x1 * v.x + x2 * v.y + x3 * v.z + x4 * v.w,
		y1 * v.x + y2 * v.y + y3 * v.z + y4 * v.w,
		z1 * v.x + z2 * v.y + z3 * v.z + z4 * v.w,
		w1 * v.x + w2 * v.y + w3 * v.z + w4 * v.w};
}
