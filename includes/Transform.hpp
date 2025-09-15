#pragma once
#include <iostream>
#include <Math.hpp>

class Transform {
	private:
		Math::Vec3 position;
		Math::Vec3 rotation;
		Math::Vec3 scale;
	public:
		Transform (): position{0,0,0}, rotation{0,0,0}, scale{1,1,1} {};
		void setPosition(float x, float y, float z) { this->position = Math::Vec3{x,y,z}; };
		void setRotate(float x, float y, float z)  { this->rotation = Math::Vec3{x,y,z}; };
		void setScale(float scale) { this->scale = Math::Vec3{scale, scale, scale}; };
		float getScale() { return this->scale.x; };
		Math::Matrix4f getModelMatrix() const;
};