#include <Renderer.hpp>
#include "shader.cpp"

void Renderer::InitObj(Mesh& obj) {
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    	throw std::runtime_error("Failed to initialize GLAD");
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto& mesh : obj.getMeshes()) {
		glGenVertexArrays(1, &mesh.VAO);
		glGenBuffers(1, &mesh.VBO);
	
		glBindVertexArray(mesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER , mesh.VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glBindVertexArray(0);
		mesh.vertexCount = mesh.vertices.size() / 11;
	}

	std::string vertexShader("srcs/mesh.vs");
	std::string fragmentShader("srcs/mesh.fs");

	shaderProgram = createShaderProgram(vertexShader, fragmentShader);
};

void Renderer::renderObj(Matrix<float>& mvp, Mesh& obj, Matrix<float> model, Camera& camera) {
	glUseProgram(this->shaderProgram);

	GLuint mvpLoc = glGetUniformLocation(this->shaderProgram, "MVP");
    GLuint modelLoc  = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(mvpLoc, 1, GL_TRUE, mvp.datal());
	glUniformMatrix4fv(modelLoc, 1, GL_TRUE, model.datal());
	GLuint kdLoc  = glGetUniformLocation(shaderProgram, "Kd");
    GLuint kaLoc  = glGetUniformLocation(shaderProgram, "Ka");
    GLuint ksLoc  = glGetUniformLocation(shaderProgram, "Ks");
    GLuint nsLoc  = glGetUniformLocation(shaderProgram, "Ns");
    GLuint niLoc  = glGetUniformLocation(shaderProgram, "Ni");
    GLuint dLoc  = glGetUniformLocation(shaderProgram, "d");
    GLuint illumLoc  = glGetUniformLocation(shaderProgram, "illum");
    GLuint hasMtlLoc  = glGetUniformLocation(shaderProgram, "hasMtl");
    GLuint lightDirLoc  = glGetUniformLocation(shaderProgram, "lightDir");
    GLuint viewPosLoc  = glGetUniformLocation(shaderProgram, "viewPos");
    GLuint lightColorLoc  = glGetUniformLocation(shaderProgram, "lightColor");

	Vector<float> cam = camera.getCameraPos();
	glUniform3f(viewPosLoc, cam.x(), cam.y(), cam.z()); //pos de la camera
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); //couleur de la lumiere;
	glUniform3f(lightDirLoc, -0.5f, -1.0f, -0.3f); //direction de la lumiere

	for (auto& mesh : obj.getMeshes()) {
		if (mesh.mat)
		{
			Vector<float> Kd = mesh.mat->getKd();
			Vector<float> Ka = mesh.mat->getKa();
			Vector<float> Ks = mesh.mat->getKs();
			glUniform1i(hasMtlLoc, 1);
			glUniform3f(kdLoc, Kd.x(), Kd.y(), Kd.z());
			glUniform3f(kaLoc, Ka.x(), Ka.y(), Ka.z());
			glUniform3f(ksLoc, Ks.x(), Ks.y(), Ks.z());
			glUniform1f(nsLoc, mesh.mat->getNs());
			glUniform1f(niLoc, mesh.mat->getNi());
			glUniform1f(dLoc, mesh.mat->getd());
			glUniform1i(illumLoc, mesh.mat->getIllum());
		}
		else
		{
			glUniform3f(kdLoc, 0.8, 0.8, 0.8);
			glUniform3f(kaLoc, 0.1, 0.1, 0.1);
			glUniform3f(ksLoc, 0.2, 0.2, 0.2);
			glUniform1f(niLoc, 1.0);
			glUniform1f(nsLoc, 32);
			glUniform1i(illumLoc, 2);
			glUniform1f(dLoc,1.0);
			glUniform1i(hasMtlLoc, 0);
			glUniform3f(kaLoc, 0.1f, 0.1f, 0.1f);
		}
		glBindVertexArray(mesh.VAO);
		glDrawArrays(GL_TRIANGLES, 0, mesh.vertexCount);
		glBindVertexArray(0);
	}
};

void Renderer::cleanup(Mesh& obj) {
	for (auto& mesh : obj.getMeshes()) {
		if (mesh.VAO)
			glDeleteVertexArrays(1, &mesh.VAO);
		if (mesh.VBO)
			glDeleteBuffers(1, &mesh.VBO);
		}
	if (this->shaderProgram)
		glDeleteProgram(this->shaderProgram);
};
