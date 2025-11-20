#include <Math.hpp>

namespace Math {
	Vec2 Vec2::normalize() const {
		float n;
		n = norm();
		if (n == 0.0f)
			return Vec2{0.0f, 0.0f};
		return Vec2{x / n, y / n};
	};
	Vec3 Vec3::normalize() const {
		float n;
		n = norm();
		if (n == 0.0f)
			return Vec3{0.0f, 0.0f, 0.0f};
		return Vec3{x / n, y / n, z / n};
	};
	void Vec3::Rotate(float angle, const Vec3& v)
 	{
		Quaternion RotationQ(angle, v);
		Quaternion ConjugateQ = RotationQ.conjugate();
		Quaternion W = RotationQ * (*this) * ConjugateQ;
		this->x = W.x;
		this->y = W.y;
		this->z = W.z;
	};
	Vec4 Vec4::normalize() const {
		float n;
		n = norm();
		if (n == 0.0f)
			return Vec4{0.0f, 0.0f, 0.0f, 0.0f};
		return Vec4{x / n, y / n, z / n, w / n};
	};

	Quaternion::Quaternion(float angle, const Vec3& v) {
		float halfAngleInRadians = ToRad(angle /2);

		float sinHalfAngle = sinf(halfAngleInRadians);
		float cosHalfAngle = cosf(halfAngleInRadians);

		x = v.x * sinHalfAngle;
		y = v.y * sinHalfAngle;
		z = v.z * sinHalfAngle;
		w = cosHalfAngle;
	};

	Quaternion::Quaternion(float x, float y, float z, float w): x(x), y(y), z(z), w(w) {};
	
	void Quaternion::normalize() {
		float norm = sqrt(x * x + y *y + z *z + w * w);
		x /= norm;
		y /= norm;
		z /= norm;
		w /= norm;
	};
	Quaternion Quaternion::conjugate() const {
		Quaternion ret(-x, -y, -z, w);
		return ret;
	};

	Quaternion operator*(const Quaternion& q, const Vec3&v) {
		float w = - (q.x * v.x) - (q.y * v.y) - (q.z * v.z);
		float x = (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
		float y = (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
		float z = (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

		Quaternion ret(x, y, z, w);
		return ret;
	}

	Quaternion operator*(const Quaternion& l, const Quaternion& r) {
		float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
		float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
		float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
		float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);


		Quaternion ret(x, y, z, w);
		return ret;
	}
	float Todegres(float rad) {
		return rad * (180.0f / M_PI);
	};

	float ToRad(float degres) {
		return degres * (M_PI / 180.0f);
	};


	Math::Matrix4f::Matrix4f(float a00, float a01, float a02, float a03,
			float a10, float a11, float a12, float a13,
			float a20, float a21, float a22, float a23,
			float a30, float a31, float a32, float a33) {
		M[0][0] = a00;M[0][1] = a01;M[0][2] = a02;	M[0][3] = a03;
		M[1][0] = a10;M[1][1] = a11;M[1][2] = a12;	M[1][3] = a13;
		M[2][0] = a20;M[2][1] = a21;M[2][2] = a22;	M[2][3] = a23;
		M[3][0] = a30;M[3][1] = a31;M[3][2] = a32;	M[3][3] = a33;
	}

	//multiplier deux matrices = multiplier la premiere ligne avec la premiere colonne et ainsi de suite
	//Attention a l'ordre de multiplication
	Math::Matrix4f Math::Matrix4f::operator*(const Math::Matrix4f& Right) const {
	Math::Matrix4f ret;
		for (unsigned int i = 0; i < 4; i++) {
			for (unsigned int j = 0; j < 4; j++)
			{
				ret.M[i][j] = 	M[i][0] * Right.M[0][j] + 
								M[i][1] * Right.M[1][j] +
								M[i][2] * Right.M[2][j] +
								M[i][3] * Right.M[3][j];
			}
		}
		return ret;
	};

	Math::Matrix4f Math::Matrix4f::identity() {
		return Math::Matrix4f(	1, 0, 0, 0,
								0, 1, 0, 0,
								0, 0, 1, 0,
								0, 0, 0, 1);
	};

	Math::Matrix4f Math::Matrix4f::translation(const Math::Vec3& position) {
		Math::Matrix4f translation = identity();
		translation.M[0][3] = position.x;
		translation.M[1][3] = position.y;
		translation.M[2][3] = position.z;
		return translation;
	};

	Math::Matrix4f Math::Matrix4f::scale(const Math::Vec3& scale) {
		Math::Matrix4f scaling = identity();
		scaling.M[0][0] = scale.x;
		scaling.M[1][1] = scale.y;
		scaling.M[2][2] = scale.z;
		scaling.M[3][3] = 1.0f;
		return scaling;
	};

	Math::Matrix4f Math::Matrix4f::rotationX(float x) {
		float c = cosf(x);
		float s = sinf(x);
		return Math::Matrix4f(
			1,0,0,0,
			0,c,-s,0,
			0,s,c,0,
			0,0,0,1
		);
	}

	Math::Matrix4f Math::Matrix4f::rotationY(float y) {
		float c = cosf(y);
		float s = sinf(y);
		return Math::Matrix4f(
			c,0,s,0,
			0,1,0,0,
			-s,0,c,0,
			0,0,0,1
		);
	};

	Math::Matrix4f Math::Matrix4f::rotationZ(float z) {
		float c = cosf(z);
		float s = sinf(z);
		return Math::Matrix4f(
			c,-s,0,0,
			s,c,0,0,
			0,0,1,0,
			0,0,0,1
		);
	};

	Math::Matrix4f Math::Matrix4f::perspective(float fov, float aspect, float near, float far) {
		float scale = 1.0f / tan(fov / 2.0f);
		return Math::Matrix4f(
			scale/aspect,0,0,0,
			0,scale,0,0,
			0,0,(far + near) / (near - far),(2 * far * near) / (near - far),
			0,0,-1,0
		);
	};

	Math::Matrix4f Math::Matrix4f::view(Math::Vec3 pos, Math::Vec3 U, Math::Vec3 V, Math::Vec3 N) {
		Math::Matrix4f RotationCam(
			U.x,U.y,U.z, 0.0f,
			V.x,V.y,V.z, 0.0f,
			N.x,N.y,N.z, 0.0f,
			0.0f,0.0f,0.0f,1.0f
		);
		
		Math::Matrix4f TranslationCam(
			1.0f,0.0f,0.0f, -pos.x,
			0.0f,1.0f,0.0f, -pos.y,
			0.0f,0.0f,1.0f, -pos.z,
			0.0f,0.0f,0.0f,1.0f
		);

		return RotationCam * TranslationCam;
	};

}