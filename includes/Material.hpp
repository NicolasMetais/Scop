#pragma once
#include <iostream>
#include <optional>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <Math.hpp>


class Material {
	private:
		Math::Vec3 Ka;
		Math::Vec3 Kd;
		Math::Vec3 Ks;
		float Ns;
		float Ni;
		float d;
		int illum;

		std::optional<unsigned int> map_Ka;
		std::optional<unsigned int> map_Kd;
		std::optional<unsigned int> map_Ks;
		std::optional<unsigned int> map_Ns;
		std::optional<unsigned int> map_d;
		std::optional<unsigned int> bump;
		std::unordered_map<std::string, std::optional<unsigned int>*> textureMap;
		std::string name;
	public:
		Material();
		void setName(const std::string& name) { this->name = name;}
		void setKa(const Math::Vec3& vec) { this->Ka = vec;}
		void setKd(const Math::Vec3& vec) { this->Kd = vec;}
		void setKs(const Math::Vec3& vec) { this->Ks = vec;}
		void setNs(const float& val) { this->Ns = val;}
		void setNi(const float& val) { this->Ni = val;}
		void setd(const float& val) { this->d = val;}
		void setIllum(const int& val) { this->illum = val;}
		void setTexture(const std::string& path, const unsigned int& val);
		void setMap_bump(const unsigned int& val) { this->bump = val;}
		std::string getName() const { return this->name;}
		Math::Vec3 getKd() const { return this->Kd;}
		Math::Vec3 getKa() const { return this->Ka;}
		Math::Vec3 getKs() const { return this->Ks;}
		float getNs() const { return this->Ns;}
		float getNi() const { return this->Ni;}
		float getd() const { return this->d;}
		int getIllum() const { return this->illum;}




};