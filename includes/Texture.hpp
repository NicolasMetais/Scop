#pragma once
#include <iostream>
#include <fstream>
#include <include/glad/glad.h>
#include <zlib.h>
#include <array>
#include <vector>
#include <cstring>

class Texture {
	private:
		GLuint id;
		u_int32_t width;
		u_int32_t height;
		void loadBMP(const std::string& path);
		void loadPNG(const std::string& path);
	public:
		Texture(): id(0), width(0), height(0) {};
		~Texture() { if (id) glDeleteTextures(1, &id); };
		GLuint getId() const { return this->id; };
		void loadTexture(const std::string& path);
		void openGlTextureGen(std::vector<unsigned char> data, int bpp);
		void scanlineInterpreter(std::vector<unsigned char>& textures, std::vector<unsigned char>& decompressed, size_t bpp);
		void bind() const { glBindTexture(GL_TEXTURE_2D, id); };
		void unbind() const { glBindTexture(GL_TEXTURE_2D, 0); };
};