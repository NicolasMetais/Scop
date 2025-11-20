#pragma once
#include <iostream>
#include <ObjModel.hpp>
#include <algorithm>
#include <include/glad/glad.h>
#include <Window.hpp>

class Renderer {
	private:
		GLuint VBO;
		GLuint VAO;
		GLsizei vertexCount;
		GLuint shaderProgram;
	public:
		Renderer() {};
		void renderObj(Math::Matrix4f& model);
		void InitObj(ObjModel& obj);
		void cleanup();
		GLuint getShader() { return shaderProgram; };
};