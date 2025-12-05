#include <Mesh.hpp>
#include <chrono>
#include <Matrix/Matrix.hpp>
#include <Window.hpp>
#include <Transform.hpp>
#include <Renderer.hpp>
#include <Camera.hpp>
#include <Texture.hpp>
#include <Skybox.hpp>
#include <Event.hpp>
#include <SDL2/SDL.h>

#define HEIGHT 1500
#define WIDTH 1500


void checkGLError(const char* where) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error at " << where << ": " << err << std::endl;
    }
}
using Clock = std::chrono::high_resolution_clock;
int main(int ac, char **av) {
    if (ac != 2)
    {
		std::cerr << "Error: Invalid format" << std::endl;
        return (1);
	}
	static uint8_t moveFlags = 0;
	auto lastTime = Clock::now();
	bool trigger = false;
	try {
		Window window(WIDTH, HEIGHT);
		Mesh teapot;
		teapot.loadObj(std::string(av[1]));
		Transform transform;
		transform.setScale(1.0f);
		transform.setPosition(0, 0, 0);
		Matrix<float> matrice = transform.getModelMatrix(); // ou imprime transform.getModelMatrix()
		Vector<float> pos = {0.0f, 0.0f ,10.0f};
		Vector<float> target = {0.0f, 0.0f ,0.0f};
		Vector<float> up = {0.0f, 1.0f ,0.0f};
		Camera camera((float)WIDTH, (float)HEIGHT, pos, target, up);
		Renderer render;
		render.InitObj(teapot);
		bool run = true;
		bool triggerTexture = false;
		Texture texture;
		texture.loadTexture("resources/test.png");
		texture.openGl2DTextureGen();
		glDisable(GL_CULL_FACE);
		Skybox sky;
		SDL_Event e;
		while (run)
		{
			auto currentTime = Clock::now();
			std::chrono::duration<float> delta = currentTime - lastTime;
			float deltaTime = delta.count();
			lastTime = currentTime;
			while (SDL_PollEvent(&e))
				event(e, transform, camera, run, triggerTexture, moveFlags, render);
			glClearColor(0.0f,0.0f,0.0f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);

			Matrix<float> model = transform.getModelMatrix();
			Matrix<float> view = camera.buildView();
			Matrix<float> projection = camera.buildProjection();
			Matrix<float> MVP = projection * view * model;

			glUseProgram(render.getShader());

			if (triggerTexture != trigger) {
				if (triggerTexture)
					render.startTransition();
				else
					render.startBackTransition();
				trigger = triggerTexture;
			}
			if(triggerTexture || render.isTransitionning())
				texture.bind();
			else
				texture.unbind();
			glUniform1i(glGetUniformLocation(render.getShader(), "useTexture"), 
            (triggerTexture || render.isTransitionning()) ? 1 : 0);
			render.renderObj(MVP, teapot, model, camera, deltaTime);
			sky.draw(camera.buildViewNoTranslation(), projection);
			SDL_GL_SwapWindow(window.getWin());
			applyMovement(camera, moveFlags);
		}
		render.cleanup(teapot);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return (0);
}