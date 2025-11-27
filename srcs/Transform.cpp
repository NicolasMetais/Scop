#include <Transform.hpp>
#include <Matrix/Matrix.hpp>
#include <utils.hpp>

Matrix<float> Transform::getModelMatrix() {
	Matrix<float> T = utils::translation(this->position);
	Matrix<float> Rx = utils::rotationX(this->rotation.x());
	Matrix<float> Ry = utils::rotationY(this->rotation.y());
	Matrix<float> Rz = utils::rotationZ(this->rotation.z());
	Matrix<float> S = identity<float>(4) * this->scale;
	return T * (Rx * Ry * Rz) * S;
};
