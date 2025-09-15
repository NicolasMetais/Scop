#include <Renderer.hpp>
#include "shader.cpp"

void Renderer::InitObj(ObjModel& obj) {
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    	throw std::runtime_error("Failed to initialize GLAD");
	glEnable(GL_DEPTH_TEST);
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);

	std::vector<float> vertices = obj.getCenteredVertices();
	vertexCount = vertices.size() / 6;
	glBindBuffer(GL_ARRAY_BUFFER , VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
};

void Renderer::renderObj(Math::Matrix4f& mvp) {
	glUseProgram(this->shaderProgram);
	GLuint mvpLoc = glGetUniformLocation(this->shaderProgram, "MVP");
	glUniformMatrix4fv(mvpLoc, 1, GL_TRUE, mvp.data());
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);
	glBindVertexArray(0);
};

void Renderer::cleanup() {
	if (this->VAO)
		glDeleteVertexArrays(1, &this->VAO);
	if (this->VBO)
		glDeleteBuffers(1, &this->VBO);
	if (this->shaderProgram)
		glDeleteProgram(this->shaderProgram);
};
