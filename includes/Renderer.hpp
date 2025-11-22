#pragma once
#include <iostream>
#include <ObjModel.hpp>
#include <algorithm>
#include <include/glad/glad.h>
#include <Window.hpp>

class Renderer {
	private:
		GLuint shaderProgram;
	public:
		Renderer() {};
		void renderObj(Math::Matrix4f& model, ObjModel& obj);
		void InitObj(ObjModel& obj);
		void cleanup(ObjModel& obj);
		GLuint getShader() { return shaderProgram; };
};