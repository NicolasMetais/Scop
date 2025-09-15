#pragma once
#include <iostream>
#include <fstream>
#include <include/glad/glad.h>

class Texture {
	private:
		GLuint id;
		int width;
		int height;
	public:
		Texture(): id(0), width(0), height(0) {};
		~Texture() { if (id) glDeleteTextures(1, &id); };
		GLuint getId() const { return id; };
		void loadBMP(const std::string& path);
		void bind() const { glBindTexture(GL_TEXTURE_2D, id); };
		void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); };

};