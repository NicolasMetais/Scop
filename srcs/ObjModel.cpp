#include <ObjModel.hpp>

ObjModel::FaceVertex ObjModel::parseFaceElement(const std::string& part)
{
	FaceVertex fv { -1, -1, -1 };
	int i = 0;
	std::stringstream ss(part);
	std::string token;
	while (std::getline(ss, token, '/')) {
		if (!token.empty()) {
			int value = std::stoi(token);
			if (i == 0) 
			{
				if (value < 0)
					fv.v = v.size() + value;
				else
					fv.v = value - 1;
			}
			if (i == 1)
			{
				if (value < 0)
					fv.vt = vt.size() + value;
				else
					fv.vt = value - 1;
			}
			if (i == 2)
			{
				if (value < 0)
					fv.vn = vn.size() + value;
				else
					fv.vn = value - 1;
			}
		}
		i++;
	}
	return fv;
}


std::string trim(const std::string& str) {
	size_t first = str.find_first_not_of(" \t\r\n");
	if (first == std::string::npos)
		return "";
	size_t last = str.find_last_not_of(" \t\r\n");
	return str.substr(first, last - first + 1);
}

void ObjModel::loadMtlFile(const std::string& fileName) {
	std::ifstream file;
	std::cout << fileName << std::endl;
	file.open("resources/" + fileName);
	if (!file.is_open())
		throw std::runtime_error("Error: cannot open mtl file");
	std::string line;
	Material current;
	while (std::getline(file, line)) {
		line = trim(line);
		if (line.empty() || line[0] == '#')
			continue ;
		std::stringstream ss(line);
		std::string token;
		ss >> token;
		if (token == "newmtl")
		{
			if (!current.getName().empty())
				materials[current.getName()] = current;
			current = Material();
			std::string name;
			ss >> name;
			current.setName(name);
		}
		else if (token == "Ka") {
			float r, g, b;
			ss >> r >> g >> b;
			current.setKa({r, g, b});
		}
		else if (token == "Kd") {
			float r, g, b;
			ss >> r >> g >> b;
			current.setKd({r, g, b});
		}
		else if (token == "Ks") {
			float r, g, b;
			ss >> r >> g >> b;
			current.setKs({r, g, b});
		}
		else if (token == "Ns") {
			float ns;
			ss >> ns;
			current.setNs(ns);
		}
		else if (token == "d") {
			float d;
			ss >> d;
			current.setd(d);
		}
		else if (token == "illum") {
			int illum;
			ss >> illum;
			current.setIllum(illum);
		}
		else if (token == "bump" || token.rfind("map_", 0) == 0) {
			std::string path;
			ss >> path;
			try { // CHECKER L'ERREUR PLUS DEEP CA DOIT BUG MAIS FLEMME LA
				unsigned int id = 0; //FONCTION DE LOAD TEXTURES A LA PLACE DU 0
				current.setTexture(token, id);
			} catch (std::runtime_error) {
				return ;
			}
		}
	}
	if (!current.getName().empty())
		materials[current.getName()] = current;
};

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
			v.push_back(vec);
		}
		else if (token == "f")
		{
			Face face;
			face.materialName = currentMaterial;
			std::string vertStr;
			while (iss >> vertStr) {
				face.fvertices.push_back(parseFaceElement(vertStr));
			}
			f.push_back(face);
		}
		else if (token == "vt")
		{
			Math::Vec2 vec;
			if ((!(iss >> vec.x >> vec.y)))
				throw std::runtime_error("Error : Invalid vt value");
			vt.push_back(vec);
		}
		else if (token == "vn")
		{
			Math::Vec3 vec;
			if ((!(iss >> vec.x >> vec.y >> vec.z)))
				throw std::runtime_error("Error : Invalid vn value");
			vn.push_back(vec);
		}
		else if (token == "mtllib")
		{
			std::string mtlFile;
			iss >> mtlFile;
			loadMtlFile(mtlFile);
		}
		else if (token == "usemtl")
		{
			std::string name;
			iss >> name;
			this->currentMaterial = name;
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
	minVert = v[0];
	maxVert = v[0];
	for (const Math::Vec3& vert : v) {
		if (vert.x < minVert.x) minVert.x = vert.x;
		if (vert.y < minVert.y) minVert.y = vert.y;
		if (vert.z < minVert.z) minVert.z = vert.z;

		if (vert.x > maxVert.x) maxVert.x = vert.x;
		if (vert.y > maxVert.y) maxVert.y = vert.y;
		if (vert.z > maxVert.z) maxVert.z = vert.z;
	}

	Math::Vec3 center = { 
		(minVert.x + maxVert.x)/2,
		(minVert.y + maxVert.y)/2,
		(minVert.z + maxVert.z)/2 
	};

	float radiusX = (maxVert.x - minVert.x) / 2.0f;
	float radiusY = (maxVert.y - minVert.y) / 2.0f;
	float radiusZ = (maxVert.z - minVert.z) / 2.0f;
	radius = std::max({radiusX, radiusY, radiusZ});

	float scaleFactor = 1.0f / radius;

	for (size_t faceIndex = 0; faceIndex < f.size(); ++faceIndex) {
		const auto& face = f[faceIndex];
		std::array<float, 3> col; //je build mon rgb
		Material* matptr = nullptr;
		if (!face.materialName.empty() && materials.count(face.materialName))
			matptr = &materials[face.materialName];
		if (matptr) {
			Math::Vec3 kd = matptr->getKd();
			col = {kd.x, kd.y, kd.z}; 
		}
		else {
			col = colors[faceIndex % colors.size()];
		}
		for (size_t j = 1; j + 1 < face.fvertices.size(); ++j) {
			FaceVertex fv0 = face.fvertices[0];
			FaceVertex fv1 = face.fvertices[j];
			FaceVertex fv2 = face.fvertices[j + 1];

			Math::Vec3 pos0 = v[fv0.v], pos1 = v[fv1.v], pos2 = v[fv2.v];

			Math::Vec3 n0, n1, n2;
			if (fv0.vn != -1) 
				n0 = vn[fv0.vn];
			else
			{
				Math::Vec3 edge1 = pos1 - pos0;
				Math::Vec3 edge2 = pos2 - pos0;

				n0 = edge1.cross(edge2).normalize();
			}

			if (fv1.vn != -1)
				n1 = vn[fv1.vn];
			else
				n1 = n0;

			if (fv2.vn != -1)
				n2 = vn[fv2.vn];
			else
				n2 = n0;

			Math::Vec2 uv0, uv1, uv2;
			bool isUV0 = false, isUV1 = false, isUV2 = false;

			if (fv0.vt != -1) { uv0 = vt[fv0.vt]; isUV0 = true;}
			if (fv1.vt != -1) { uv1 = vt[fv1.vt]; isUV1 = true;}
			if (fv2.vt != -1) { uv2 = vt[fv2.vt]; isUV2 = true;}


			for (int j = 0; j < 3; ++j) {
				Math::Vec3 pos, norm;
				Math::Vec2 uv;
				bool isUV = false;

				if (j == 0) { pos = pos0; norm = n0; uv = uv0; isUV = isUV0;}
				else if (j == 1) { pos = pos1; norm = n1; uv = uv1; isUV = isUV1;}
				else { pos = pos2; norm = n2; uv = uv2; isUV = isUV2;}

				float x = (pos.x - center.x) * scaleFactor;
				float y = (pos.y - center.y) * scaleFactor;
				float z = (pos.z - center.z) * scaleFactor;

				centeredVertices.push_back(x);
				centeredVertices.push_back(y);
				centeredVertices.push_back(z);

				centeredVertices.push_back(col[0]);
				centeredVertices.push_back(col[1]);
				centeredVertices.push_back(col[2]);

				//a remettre quand les normales sont codees
				// centeredVertices.push_back(norm.x);
				// centeredVertices.push_back(norm.y);
				// centeredVertices.push_back(norm.z);
				if (isUV)
				{
					centeredVertices.push_back(uv.x);
					centeredVertices.push_back(uv.y);
				}
				else
				{
					centeredVertices.push_back((0.5f + atan2(z, x) / (2.0f * M_PI) * 20.0f));
					centeredVertices.push_back((0.5f - asin(y / radius) / M_PI) * 20.0f);
				}
			}
		}
	}
}