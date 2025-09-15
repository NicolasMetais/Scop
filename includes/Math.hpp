#pragma once
#include <iostream>
#include <cmath>
#include <math.h>

namespace Math {
	float Todegres(float rad);
	float ToRad(float degres);
	struct Vec2 { 
		float x, y; 
		Vec2() : x(0), y(0) {};
		Vec2(float x, float y) : x(x), y(y) {};
		float norm() const { return std::sqrt(x * x + y * y);};
		Vec2 normalize() const;
		Vec2 operator*(float f) const { return {x * f, y *f}; };
		Vec2& operator+=(const Vec2& v) { x += v.x; y += v.y; return *this; };
		Vec2& operator-=(const Vec2& v) { x -= v.x; y -= v.y; return *this; };
		Vec2& operator*=(const Vec2& v) { x *= v.x; y *= v.y; return *this; };
		Vec2& operator/=(const Vec2& v) { x /= v.x; y /= v.y; return *this; };
	};
	struct Vec3 {
		float x, y ,z;
		Vec3() : x(0), y(0), z(0) {};
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {};
		float norm() const { return std::sqrt(x * x + y * y + z * z);};
		Vec3 normalize() const;
		void Rotate(float angle, const Vec3& v);
		Vec3 operator*(float f) const { return {x * f, y *f, z * f}; };
		Vec3 operator+(Vec3 v) const { return {v.x + x, v.y + y, v.z + z}; };
		Vec3 operator-(Vec3 v) const { return {v.x - x, v.y - y, v.z - z}; };
		Vec3& operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; };
		Vec3& operator+=(float& f) { x += f; y += f; z += f; return *this; };
		Vec3& operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; };
		Vec3& operator-=(float& f) { x -= f; y -= f; z -= f; return *this; };
		Vec3& operator*=(const Vec3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; };
		Vec3& operator*=(float& f) { x *= f; y *= f; z *= f; return *this; };
		Vec3& operator/=(const Vec3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; };
		Vec3& operator/=(float& f) { x /= f; y /= f; z /= f; return *this; };
		Vec3 cross(const Vec3& v) const { return { (y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x)}; };
	};
	struct Vec4 {
		float x, y ,z, w;
		Vec4() : x(0), y(0), z(0), w(0) {};
		Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
		float norm() const { return std::sqrt(x * x + y * y + z * z + w * w);};
		Vec4 normalize() const;
		Vec4 operator*(float f) const { return {x * f, y *f, z * f, w * f}; };
		Vec4& operator+=(const Vec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; };
		Vec4& operator-=(const Vec4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; };
		Vec4& operator*=(const Vec4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; };
		Vec4& operator/=(const Vec4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; };
	};
	struct Quaternion {
		float x, y, z, w;
		Quaternion(float angle, const Vec3& v);
		Quaternion(float x, float y, float z, float w);
		void normalize();
		Quaternion conjugate() const;
		Vec3 ToDegrees();
	};
	Quaternion operator*(const Quaternion& q, const Vec3&v) ;
	Quaternion operator*(const Quaternion& l, const Quaternion& r);
	class Matrix4f {
		public:
			float M[4][4];
		Matrix4f() {};
		Matrix4f(float a00, float a01, float a02, float a03,
			float a10, float a11, float a12, float a13,
			float a20, float a21, float a22, float a23,
			float a30, float a31, float a32, float a33);
		Matrix4f operator*(const Matrix4f& Right) const;
		static Matrix4f identity();
		static Matrix4f translation(const Math::Vec3& position);
		static Matrix4f scale(const Math::Vec3& scale);
		static Matrix4f rotationX(float x);
		static Matrix4f rotationY(float y);
		static Matrix4f rotationZ(float z);
		static Matrix4f perspective(float fov, float aspect, float near, float far);
		static Matrix4f view(Math::Vec3 pos, Math::Vec3 U, Math::Vec3 V, Math::Vec3 N);
		const float* data() const { return &M[0][0]; };
	};
}