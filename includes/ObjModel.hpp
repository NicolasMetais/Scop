#pragma once
#include <iostream>
#include <Math.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <array>
#include <include/glad/glad.h>

class ObjModel {
	private:
		GLuint VAO, VBO;
		std::vector<Math::Vec3> vertices;
		std::vector<std::vector<int>> faces;
		std::vector<float> centeredVertices;
		Math::Vec3 minVert;
		Math::Vec3 maxVert;
		float radius;
	public:
		ObjModel () {};
		void loadObj(const std::string& filename);
		const std::vector<Math::Vec3>& getVertices() const { return vertices; };
		const std::vector<std::vector<int>>& getFaces() const { return faces; };
		const std::vector<float>& getCenteredVertices() const { return centeredVertices; };
		const Math::Vec3& getMinVertices() const { return minVert; };
		const Math::Vec3& getMaxVertices() const { return maxVert; };
		const float& getRadius() const { return radius; };
		GLuint getVAO()const { return VAO; };
		GLuint getVBO()const { return VBO; };
};