#pragma once
#include <iostream>
#include <Math.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <array>
#include <include/glad/glad.h>
#include <Material.hpp>
#include <unordered_map>

struct MaterialMesh {
	Material *mat;
	GLuint VAO, VBO;
	size_t vertexCount;
	std::vector<float> vertices;
};

class Mesh {
	private:
		std::vector<Math::Vec3> v;
		std::vector<Math::Vec2> vt;
		std::vector<Math::Vec3> vn;
		std::vector<float> centeredVertices;
		std::vector<MaterialMesh> meshes;
		Math::Vec3 minVert;
		Math::Vec3 maxVert;
		Math::Vec3 center;
		std::vector<std::array<float, 3>> colors;
		float radius;
		float scaleFactor;
		std::string currentMaterial;
		struct FaceVertex {
			int v;
			int vt;
			int vn;
		};
		struct Face {
			std::vector<FaceVertex> fvertices;
			std::string materialName;
		};
		std::unordered_map <std::string, Material> materials;
		std::vector<Face> f;
	public:
		Mesh () {};
		void loadObj(const std::string& fileName);
		void loadMtlFile(const std::string& mtlname);
		FaceVertex parseFaceElement(const std::string& part);

		std::vector<MaterialMesh>& getMeshes() { return meshes; };
		const std::vector<Math::Vec3>& getVertices() const { return v; };
		const std::vector<float>& getCenteredVertices() const { return centeredVertices; };
		const Math::Vec3& getMinVertices() const { return minVert; };
		const Math::Vec3& getMaxVertices() const { return maxVert; };
		const float& getRadius() const { return radius; };
		void parseObjFile(const std::string& fileName);
		void CenterAndNormalize();
		void BuildRenderMesh();
		void parseVertexLine(std::istringstream& iss);
		void parseTexCoordLine(std::istringstream& iss);
		void parseNormalLine(std::istringstream& iss);
		void parseFaceLine(std::istringstream& iss);
		void parseMtlUseLine(std::istringstream& iss);
		void parseMtlLibLine(std::istringstream& iss);
		Math::Vec3 computeFaceNormal(const Math::Vec3& p0, const Math::Vec3& p1, const Math::Vec3& p2) const;
		Math::Vec3 getVertexNormal(const FaceVertex& fv, const Math::Vec3& defaultNormal) const;
		Math::Vec2 getVertexUV(const FaceVertex& fv, const Math::Vec3& pos) const;
		void pushVertex(MaterialMesh& mesh, const Math::Vec3& pos, const std::array<float, 3>& color, const Math::Vec3& normal, const Math::Vec2& uv);
		
};