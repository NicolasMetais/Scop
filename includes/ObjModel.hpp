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

class ObjModel {
	private:
		GLuint VAO, VBO;
		std::vector<Math::Vec3> v;
		std::vector<Math::Vec2> vt;
		std::vector<Math::Vec3> vn;
		std::vector<float> centeredVertices;
		Math::Vec3 minVert;
		Math::Vec3 maxVert;
		float radius;
		struct FaceVertex {
			int v;
			int vt;
			int vn;
		};
		struct Face {
			std::vector<FaceVertex> fvertices;
		};
		std::unordered_map <std::string, Material> materials;
		std::vector<Face> f;
	public:
		ObjModel () {};
		void loadObj(const std::string& filename);
		void loadMtlFile(const std::string& mtlname);
		const std::vector<Math::Vec3>& getVertices() const { return v; };
		const std::vector<float>& getCenteredVertices() const { return centeredVertices; };
		const Math::Vec3& getMinVertices() const { return minVert; };
		const Math::Vec3& getMaxVertices() const { return maxVert; };
		const float& getRadius() const { return radius; };
		FaceVertex parseFaceElement(const std::string& part);
		GLuint getVAO()const { return VAO; };
		GLuint getVBO()const { return VBO; };
};