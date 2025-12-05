#pragma once
#include <iostream>
#include <Mesh.hpp>
#include <algorithm>
#include <include/glad/glad.h>
#include <Window.hpp>
#include <Camera.hpp>

class Renderer {
	private:
		GLuint shaderProgram;
		bool transitionning = false;
		bool transitionDir = true;
		float transition = 0.0f;
	public:
		Renderer() {};
		void renderObj(Matrix<float>& mvp, Mesh& obj, Matrix<float> model, Camera& camera, float deltaTime);
		void InitObj(Mesh& obj);
		void cleanup(Mesh& obj);
		GLuint getShader() { return shaderProgram; };
		bool isTransitionning() const { return transitionning; };
		void startTransition() 
		{
			this->transition = 0.0f;
			transitionning = true; 
			transitionDir = true;
		};
		void startBackTransition() { 
			this->transition = 1.0f;
			transitionning = true; 
			transitionDir = false;
		};
};