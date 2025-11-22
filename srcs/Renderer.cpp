#include <Renderer.hpp>
#include "shader.cpp"

void Renderer::InitObj(ObjModel& obj) {
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    	throw std::runtime_error("Failed to initialize GLAD");
	
	glEnable(GL_DEPTH_TEST);

	for (auto& mesh : obj.getMeshes()) {
		glGenVertexArrays(1, &mesh.VAO);
		glGenBuffers(1, &mesh.VBO);
	
		glBindVertexArray(mesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER , mesh.VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);
		mesh.vertexCount = mesh.vertices.size() / 8;
	}

	shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
};

void Renderer::renderObj(Math::Matrix4f& mvp, ObjModel& obj) {
	glUseProgram(this->shaderProgram);

	GLuint mvpLoc = glGetUniformLocation(this->shaderProgram, "MVP");
	glUniformMatrix4fv(mvpLoc, 1, GL_TRUE, mvp.data());
	GLuint kdLoc  = glGetUniformLocation(shaderProgram, "Kd");
    GLuint kaLoc  = glGetUniformLocation(shaderProgram, "Ka");
    GLuint ksLoc  = glGetUniformLocation(shaderProgram, "Ks");
    GLuint nsLoc  = glGetUniformLocation(shaderProgram, "Ns");
    GLuint niLoc  = glGetUniformLocation(shaderProgram, "Ni");
    GLuint dLoc  = glGetUniformLocation(shaderProgram, "d");
    GLuint illumLoc  = glGetUniformLocation(shaderProgram, "illum");
	for (auto& mesh : obj.getMeshes()) {
		Math::Vec3 Kd = mesh.mat->getKd();
		Math::Vec3 Ka = mesh.mat->getKa();
		Math::Vec3 Ks = mesh.mat->getKs();
		float Ns = mesh.mat->getNs();
		float Ni = mesh.mat->getNi();
		float d = mesh.mat->getd();
		float illum = mesh.mat->getIllum();
		glUniform3f(kdLoc, Kd.x, Kd.y, Kd.z);
		glUniform3f(kaLoc, Ka.x, Ka.y, Ka.z);
		glUniform3f(ksLoc, Ks.x, Ks.y, Ks.z);
		glUniform1f(nsLoc, mesh.mat->getNs());
		glUniform1f(niLoc, mesh.mat->getNi());
		glUniform1f(dLoc, mesh.mat->getd());
		glUniform1i(illumLoc, mesh.mat->getIllum());
		glBindVertexArray(mesh.VAO);

		glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
		glBindVertexArray(0);
	}
};

void Renderer::cleanup(ObjModel& obj) {
	for (auto& mesh : obj.getMeshes()) {
		if (mesh.VAO)
			glDeleteVertexArrays(1, &mesh.VAO);
		if (mesh.VBO)
			glDeleteBuffers(1, &mesh.VBO);
		}
	if (this->shaderProgram)
		glDeleteProgram(this->shaderProgram);
};
