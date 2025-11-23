#pragma once
#include <iostream>
#include <Mesh.hpp>
#include <algorithm>
#include <include/glad/glad.h>
#include <Window.hpp>

class Renderer {
	private:
		GLuint shaderProgram;
	public:
		Renderer() {};
		void renderObj(Math::Matrix4f& mvp, Mesh& obj, Math::Matrix4f model);
		void InitObj(Mesh& obj);
		void cleanup(Mesh& obj);
		GLuint getShader() { return shaderProgram; };
};