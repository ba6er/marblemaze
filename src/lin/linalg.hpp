#pragma once

namespace lin {

	inline constexpr float Pi  = 3.1415926535897932385f;
	inline constexpr float Tau = 6.2831853071795864769f;
	inline constexpr float DegToRad = Pi / 180;
	inline constexpr float RadToDeg = 180 / Pi;

	class Vec2 {

	public:
		Vec2 operator-() const;
		Vec2 operator+(Vec2 v) const;
		Vec2 operator-(Vec2 v) const;
		Vec2 operator*(float n) const;
		Vec2 operator/(float n) const;

		float x, y;
	};

	class Vec4 {

	public:
		Vec4 operator-() const;
		Vec4 operator+(Vec4 v) const;
		Vec4 operator-(Vec4 v) const;
		Vec4 operator*(float n) const;
		Vec4 operator/(float n) const;

		float x, y, z, w;
	};

	class Vec3 {

	public:
		Vec3 operator-() const;
		Vec3 operator+(Vec3 v) const;
		Vec3 operator-(Vec3 v) const;
		Vec3 operator*(float n) const;
		Vec3 operator/(float n) const;

		void operator+=(Vec3 v);
		void operator-=(Vec3 v);
		void operator*=(float n);
		void operator/=(float n);

		Vec3  normalize() const;
		Vec3  cross(Vec3 v) const;
		float dot(Vec3 v) const;
		float length() const;

		float x, y, z;
	};

	class Mat4 {

	public:
		static Mat4 Identity();
		static Mat4 Scale(float x, float y, float z);
		static Mat4 Scale(Vec3 scalar);
		static Mat4 Rotate(float a, float nx, float ny, float nz);
		static Mat4 Rotate(float a, Vec3 axis);
		static Mat4 Translate(float x, float y, float z);
		static Mat4 Translate(Vec3 translation);

		static Mat4 LookAt(Vec3 eye, Vec3 target, Vec3 up);
		static Mat4 Project2d(float left, float right, float bottom, float top, float near, float far);
		static Mat4 Project3d(float fov, float aspect, float near, float far);

		Mat4 operator+(const Mat4& m) const;
		Mat4 operator-(const Mat4& m) const;
		Mat4 operator*(const Mat4& m) const;

		Mat4 transpose() const;
		Mat4 inverse() const;

		Vec3 operator*(Vec3 v) const;
		Vec4 operator*(Vec4 v) const;

		float x1, x2, x3, x4;
		float y1, y2, y3, y4;
		float z1, z2, z3, z4;
		float w1, w2, w3, w4;
	};
}
