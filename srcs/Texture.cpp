#include <Texture.hpp>

void Texture::loadTexture(const std::string& path) {
	if (path.size() >= 4 && path.substr(path.size() - 4) == ".bmp")
		loadBMP(path);
	else if (path.size() >= 4 && path.substr(path.size() - 4) == ".png")
		loadPNG(path);
	else
		throw std::runtime_error("Error: wrong extension file. Only bmp and png are handled for textures files here");
};


void Texture::loadBMP(const std::string& path) {
	std::ifstream file(path, std::ios::binary); 
	if (!file.is_open())
		throw std::runtime_error("bmp file opening error");
	unsigned char header[54];
	file.read(reinterpret_cast<char*>(header), 54);
	width = *reinterpret_cast<u_int32_t*>(&header[18]);
	height = *reinterpret_cast<u_int32_t*>(&header[22]);
	short bpp = *reinterpret_cast<short*>(&header[28]);
	int offset = *reinterpret_cast<int*>(&header[10]);
	
	if (bpp != 24 && bpp != 32)
		throw std::runtime_error("only BMP 24 & 32 bits suppported");
		
	int bytesPerPixel = bpp / 8;
	int imgSize = width * height * bytesPerPixel;
	
	file.seekg(offset, std::ios::beg);
	unsigned char *data = new unsigned char[imgSize];
	file.read(reinterpret_cast<char*>(data), imgSize);
	for (int y = 0; y < (int)height / 2; ++y)
	{
		for (int x = 0; x < (int)width * bytesPerPixel; ++x)
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

uint32_t readUint32FromFile(std::ifstream &file) {
	unsigned char bytes[4];
	file.read(reinterpret_cast<char *>(bytes), 4);
	if (file.gcount() != 4)
		throw std::runtime_error("Invalid uint read");
	return ((uint32_t(bytes[0]) << 24) 
			| (uint32_t(bytes[1]) << 16)
			| (uint32_t(bytes[2]) << 8) 
			| (uint32_t(bytes[3])));
};

uint32_t readUint32FromBuff(unsigned char *data) {
	return ((uint32_t(data[0]) << 24) 
			| (uint32_t(data[1]) << 16)
			| (uint32_t(data[2]) << 8) 
			| (uint32_t(data[3])));
};

std::vector<unsigned char> inflatePNG(const std::vector<unsigned char>& compressedData, int size) {
	std::vector<unsigned char> out(size);

	z_stream stream;
	std::memset(&stream, 0, sizeof(stream));

	stream.next_in = const_cast<Bytef*>(compressedData.data());
	stream.avail_in = compressedData.size();

	stream.next_out = out.data();
	stream.avail_out = out.size();

	if (inflateInit(&stream) != Z_OK)
		throw std::runtime_error("inflateInit failed");
	
	int ret = inflate(&stream, Z_FINISH);
	if (ret != Z_STREAM_END)
	{
		inflateEnd(&stream);
		throw std::runtime_error("inflate failed");
	}
	inflateEnd(&stream);
	return out;
}

int samplePerPixel(uint8_t colorType) {
	switch (colorType)
	{
	case 0: //GrayScale
		return 1; 
	case 2: //RGB
		return 3;
	case 3: //Indexed-color (une palette)
		return 1;
	case 4: //Grayscale + alpha
		return 2;
	case 6: //RGBA
		return 4;
	default:
		throw std::runtime_error("Invalid PNG colorType");
	}
}

void scanlineInterpreter(std::vector<unsigned char> textures, std::vector<unsigned char> decompressed, int bpp, u_int32_t width, u_int32_t height) {
	unsigned char *prev_line;
	int stride = 1 + width * bpp; //taille reel d'une scanline +1 byte pour le byte de filtre
	for(size_t i = 0; i < height; i++) {
		unsigned char *scanline = decompressed.data() + i * stride;
		unsigned char filter = scanline[0];
		unsigned char * line = new unsigned char[width * bpp];
		for (size_t j = 0; j < width * bpp; j++) {
			unsigned char left = 0;
			unsigned char above = 0;
			unsigned char upper_left = 0;

			if (i >= bpp)
				left = line[j - bpp];
			if (i > 0)
				above = prev_line[j];
			if (i >= bpp && i > 0)
				upper_left = prev_line[j - bpp];
		}
	}
}


void Texture::loadPNG(const std::string& path) {
	uint8_t bitDepth;
	uint8_t colorType;
	uint8_t compressionMethod;
	uint8_t filterMethod;
	uint8_t interlaceMethod;

	std::ifstream file(path, std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("png file opening error");
	std::array<unsigned char, 8> signature{};
	file.read(reinterpret_cast<char *>(signature.data()), 8);
	if (file.gcount() != 8)
		throw std::runtime_error("Invalid signature");
	//METTRE UNE ERREUR SI MOINS DE 8 CHAR
	unsigned char expected[8] = {
		0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A
	}; //signature attendu sur tout les png
	if (!std::equal(std::begin(signature), std::end(signature), std::begin(expected)))
		throw std::runtime_error("Invalid signature");
	int i = 0;
	std::vector<unsigned char> compressedData;
	while (true) {
		uint32_t length = readUint32FromFile(file); //calcul de la lenght de data
		char type[5] = {0};
		file.read(type, 4); //description du type de data et de ce que je doit en faire
		if (file.gcount() != 4)
			throw std::runtime_error("Invalid chunk type");
		if (i == 0 && std::strncmp(type, "IHDR", 4) != 0)
			throw std::runtime_error("IHDR must be the first chunk");
		std::vector<unsigned char> data(length);
		file.read(reinterpret_cast<char*>(data.data()), length);
		if (file.gcount() != length)
			throw std::runtime_error("invalid chunk");
		if (std::strncmp(type, "IDAT", 4) == 0)
			compressedData.insert(compressedData.end(), data.begin(), data.end());
		else if (std::strncmp(type, "IHDR", 4) == 0 && i == 0)
		{
			this->width = readUint32FromBuff(data.data());
			this->height = readUint32FromBuff(data.data() + 4);
			bitDepth = data[8];
			colorType = data[9];
			compressionMethod = data[10];
			filterMethod = data[11];
			interlaceMethod = data[12];
		}
		else if (std::strncmp(type, "IEND", 4) == 0)
			break ;
		uint32_t crc = readUint32FromFile(file); //Je stock le crc. il sert a verif l'integrite des donne apres loading
		uint32_t calc_crc = crc32(0L, Z_NULL, 0); //init d'un crc vide
		calc_crc = crc32(calc_crc, reinterpret_cast<const Bytef*>(type), 4); //je met le type
		calc_crc = crc32(calc_crc, data.data(), data.size()); //je met la data
		if (calc_crc != crc) // je compare si les deux crc sont les meme
			throw std::runtime_error("CRC mismatch"); //check la validite de la data avec crc
		i++;
	}
	int bpp = (bitDepth * samplePerPixel(colorType) + 7) / 8;
	std::vector<unsigned char> decompressed = inflatePNG(compressedData, bpp);
	std::vector<unsigned char> textures;
	scanlineInterpreter(textures, decompressed, bpp, width, height);
};

