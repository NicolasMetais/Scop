#pragma once
#include <iostream>
#include <Mesh.hpp>
#include <algorithm>
#include <include/glad/glad.h>
#include <Window.hpp>
#include <Camera.hpp>

struct ShaderLocations {
	   // Matrices
    GLint MVP;
    GLint model;

    // Material
    GLint Kd;
    GLint Ka;
    GLint Ks;
    GLint Ns;
    GLint Ni;
    GLint d;
    GLint illum;
    GLint hasMtl;
	
    // Textures (samplers)
    GLint map_Kd;
    GLint isMap_Kd;

    GLint map_Ka;
    GLint isMap_Ka;

    GLint map_Ks;
    GLint isMap_Ks;

    GLint map_Ns;
    GLint isMap_Ns;

    GLint map_d;
    GLint isMap_d;

    GLint bump;
    GLint isBump;

	GLint scopTexture;

    // Light
    GLint lightDir;
    GLint viewPos;
    GLint lightColor;

    // Misc
    GLint transition;
};

class Renderer {
	private:
		GLuint shaderProgram;
		bool transitionning = false;
		bool transitionDir = true;
		float transition = 0.0f;
		ShaderLocations loc;
		Texture ScopTexture;
	public:
		Renderer();
		void renderObj(Matrix<float>& mvp, Mesh& obj, Matrix<float> model, Camera& camera, float deltaTime, bool& triggerTexture);
		void InitObj(Mesh& obj);
		void cleanup(Mesh& obj);
		GLuint getShader() { return shaderProgram; };
		void bindTexture(int& texSlot, GLuint loc, GLuint , const std::optional<Texture>& texture);
		bool isTransitionning() const { return transitionning; };
		void startTransition() 
		{
			this->transition = 0.0f;
			transitionning = true; 
			transitionDir = true;
		};
		void startBackTransition() { 
			this->transition = 1.0f;
			transitionning = true; 
			transitionDir = false;
		};
};