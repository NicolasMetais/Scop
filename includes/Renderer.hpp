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
	public:
		Renderer() {};
		void renderObj(Matrix<float>& mvp, Mesh& obj, Matrix<float> model, Camera& camera);
		void InitObj(Mesh& obj);
		void cleanup(Mesh& obj);
		GLuint getShader() { return shaderProgram; };
};