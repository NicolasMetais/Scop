#include <Transform.hpp>

Math::Matrix4f Transform::getModelMatrix() const {
	Math::Matrix4f T = Math::Matrix4f::translation(this->position);
	Math::Matrix4f Rx = Math::Matrix4f::rotationX(this->rotation.x);
	Math::Matrix4f Ry = Math::Matrix4f::rotationY(this->rotation.y);
	Math::Matrix4f Rz = Math::Matrix4f::rotationZ(this->rotation.z);
	Math::Matrix4f S = Math::Matrix4f::scale(this->scale);
	return T * (Rx * Ry * Rz) * S;
};
