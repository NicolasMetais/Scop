#include <utils.hpp>

Matrix<float> utils::rotationX(float x) {
		float c = cosf(x);
		float s = sinf(x);
		return Matrix<float> ({
			{1,0,0,0},
			{0,c,-s,0},
			{0,s,c,0},
			{0,0,0,1}}
		);
};

Matrix<float> utils::rotationY(float y) {
		float c = cosf(y);
		float s = sinf(y);
		return Matrix<float>({
			{c,0,s,0},
			{0,1,0,0},
			{-s,0,c,0},
			{0,0,0,1}}
		);
};

Matrix<float> utils::rotationZ(float z) {
		float c = cosf(z);
		float s = sinf(z);
		return Matrix<float>({
			{c,-s,0,0},
			{s,c,0,0},
			{0,0,1,0},
			{0,0,0,1}}
		);
};

Matrix<float> utils::perspective(float fov, float aspect, float near, float far) {
		float scale = 1.0f / tan(fov / 2.0f);
		return Matrix<float>({
			{scale/aspect,0,0,0},
			{0,scale,0,0},
			{0,0,(far + near) / (near - far),(2 * far * near) / (near - far)},
			{0,0,-1,0}}
		);
};

Matrix<float> utils::view(Vector<float> pos, Vector<float> U, Vector<float> V, Vector<float> N) {
		Matrix<float> RotationCam({
			{U.x(),U.y(),U.z(), 0.0f},
			{V.x(),V.y(),V.z(), 0.0f},
			{N.x(),N.y(),N.z(), 0.0f},
			{0.0f,0.0f,0.0f,1.0f}}
		);
		
		Matrix<float> TranslationCam({
			{1.0f,0.0f,0.0f, -pos.x()},
			{0.0f,1.0f,0.0f, -pos.y()},
			{0.0f,0.0f,1.0f, -pos.z()},
			{0.0f,0.0f,0.0f,1.0f}}
		);

		return RotationCam * TranslationCam;
};

Matrix<float> utils::translation(const Vector<float>& position) {
		Matrix<float> translation = identity<float>(4);
		translation[0][3] = position.x();
		translation[1][3] = position.y();
		translation[2][3] = position.z();
		return translation;
};

float utils::Todegres(float rad) {
		return rad * (180.0f / M_PI);
	};

float utils::ToRad(float degres) {
		return degres * (M_PI / 180.0f);
	};