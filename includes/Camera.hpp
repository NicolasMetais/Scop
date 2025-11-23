#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <Mesh.hpp>

class Camera {
	private:
		Math::Vec3 cameraPos;
		Math::Vec3 target;
		Math::Vec3 up;
		Math::Vec3 U;
		Math::Vec3 V;
		Math::Vec3 N;
		Math::Vec2 mousePos;
		int winWidth;
		int winHeight;
		float aspect;
		float fov;
		float near;
		float far;
		float speed;
		float angleH;
		float angleV;
		bool upperEdge;
		bool leftEdge;
		bool rightEdge;
		bool lowerEdge;
	public:
		Camera(float w, float h, Math::Vec3 pos);
		Camera(float w, float h, Math::Vec3& pos, Math::Vec3& target, Math::Vec3& up);
		Math::Matrix4f buildProjection();
		Math::Matrix4f updateProjection(float w, float h);
		Math::Matrix4f buildView();
		void setFar(const Mesh& obj);
		void moveForward();
		void moveBackward();
		void moveLeft();
		void moveRight();
		void moveUp();
		void moveDown();
		void speedUp();
		void speedDown();
		void cameraInit();
		void cameraUpdate();
		void mouseActions();
		Math::Vec2 getMousePos() const { return mousePos; };
};