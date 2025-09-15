#include <ObjModel.hpp>

void ObjModel::loadObj(const std::string& filename) {
	if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".obj")
		throw std::runtime_error("Error: wrong obj extension file");
	std::ifstream file;
	file.open(filename);
	if (!file.is_open())
		throw std::runtime_error("Error: cannot open file");
	std::string line;
	while(std::getline(file, line))
	{
		if (line.empty())
			continue ; 
		std::istringstream iss(line);
		std::string token;
		iss >> token;
		if (token == "v")
		{
			Math::Vec3 vec;
			if (!(iss >> vec.x >> vec.y >> vec.z))
				throw std::runtime_error("Error : Invalid v value");
			vertices.push_back(vec);
		}
	}
	file.clear();
	file.seekg(0, std::ios::beg);
	while(std::getline(file, line))
	{
		if (line.empty())
			continue ; 
		std::istringstream iss(line);
		std::string token;
		iss >> token;
		if (token == "f")
		{
			std::vector<int> indices;
			std::string vertStr;
			while(iss >> vertStr) {
				int idx = std::stoi(vertStr);
				indices.push_back(idx);
			}
			if (indices.size() < 3)
				throw std::runtime_error("Face must have 3 vertices at least");
			for (size_t j = 1; j + 1 < indices.size(); ++j)
				faces.push_back({ indices[0], indices[j] ,indices[j + 1] });
		}
	}
	file.close();

	std::vector<std::array<float, 3>> colors = {
        {0.0f, 0.0f, 0.0f},
        {0.2f, 0.2f, 0.2f},
        {0.4f, 0.4f, 0.4f},
        {0.6f, 0.6f, 0.6f},
        {0.8f, 0.8f, 0.8f},
        {0.9f, 0.9f, 0.9f}
	};
	minVert = vertices[0];
	maxVert = vertices[0];
	for (const Math::Vec3& vert : vertices) {
		if (vert.x < minVert.x) minVert.x = vert.x;
		if (vert.y < minVert.y) minVert.y = vert.y;
		if (vert.z < minVert.z) minVert.z = vert.z;
		if (vert.x > maxVert.x) maxVert.x = vert.x;
		if (vert.y > maxVert.y) maxVert.y = vert.y;
		if (vert.z > maxVert.z) maxVert.z = vert.z;
	}
	Math::Vec3 center = { (minVert.x + maxVert.x)/2, (minVert.y + maxVert.y)/2, (minVert.z + maxVert.z)/2 };
	float radiusX = (maxVert.x - minVert.x) / 2.0f;
	float radiusY = (maxVert.y - minVert.y) / 2.0f;
	float radiusZ = (maxVert.z - minVert.z) / 2.0f;
	radius = std::max({radiusX, radiusY, radiusZ});
	float scaleFactor = 1.0f / radius;
	for (size_t index = 0; index < faces.size(); ++index)
	{
		const auto& col = colors[index % colors.size()];
		for (int i : faces[index])
		{
			const Math::Vec3& vtx = vertices[i - 1];
			float x = (vtx.x - center.x) * scaleFactor;
			float y = (vtx.y - center.y) * scaleFactor;
			float z = (vtx.z - center.z) * scaleFactor;

			centeredVertices.push_back(x);
			centeredVertices.push_back(y);
			centeredVertices.push_back(z);
			centeredVertices.push_back(col[0]);
			centeredVertices.push_back(col[1]);
			centeredVertices.push_back(col[2]);
			centeredVertices.push_back((0.5f + atan2(z, x) / (2.0f * M_PI) * 20.0f));
			centeredVertices.push_back((0.5f - asin(y / radius) / M_PI) * 20.0f);
		}
	}
}