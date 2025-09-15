#include <Texture.hpp>

void Texture::loadBMP(const std::string& path) {
	if (path.size() < 4 || path.substr(path.size() - 4) != ".bmp")
		throw std::runtime_error("Error: wrong bmp extension file");
	std::ifstream file(path, std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("bmp file error");
	
	unsigned char header[54];
	file.read(reinterpret_cast<char*>(header), 54);
	width = *reinterpret_cast<int*>(&header[18]);
	height = *reinterpret_cast<int*>(&header[22]);
	short bpp = *reinterpret_cast<short*>(&header[28]);
	int offset = *reinterpret_cast<int*>(&header[10]);
	
	if (bpp != 24 && bpp != 32)
		throw std::runtime_error("only BMP 24 & 32 bits suppported");
		
	int bytesPerPixel = bpp / 8;
	int imgSize = width * height * bytesPerPixel;
	
	file.seekg(offset, std::ios::beg);
	unsigned char *data = new unsigned char[imgSize];
	file.read(reinterpret_cast<char*>(data), imgSize);
	for (int y = 0; y < height / 2; ++y)
	{
		for (int x = 0; x < width * bytesPerPixel; ++x)
			std::swap(data[y * width * bytesPerPixel + x], 
					data[(height - 1 - y) * width * bytesPerPixel + x]);
	}
	file.close();
	
	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);

	//sert a repeter la texture plusieurs fois 
	//si elle est trop petite par rapport au model
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//gestion de la texture si elle est scale en plus petit
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//gestion de la texture si elle est scale en plus grand
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum format = (bytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	GLenum inputFormat = (bytesPerPixel == 4) ? GL_BGRA : GL_BGR;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, inputFormat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	delete[] data;
};
