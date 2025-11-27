#include <Mesh.hpp>
#include <Matrix/Matrix.hpp>
#include <Window.hpp>
#include <Transform.hpp>
#include <Renderer.hpp>
#include <Camera.hpp>
#include <Texture.hpp>
#include <Event.hpp>
#include <SDL2/SDL.h>

#define HEIGHT 1500
#define WIDTH 1500

int main(int ac, char **av) {
    if (ac != 2)
    {
		std::cerr << "Error: Invalid format" << std::endl;
        return (1);
	}
	try {
		Window window(WIDTH, HEIGHT);
		Mesh teapot;
		teapot.loadObj(std::string(av[1]));
		Transform transform;
		transform.setScale(1.0f);
		transform.setPosition(0, 0, 0);
		Vector<float> pos = {0.0f, 0.0f ,5.0f};
		Vector<float> target = {0.0f, 0.0f ,1.0f};
		Vector<float> up = {0.0f, 1.0f ,0.0f};
		Camera camera((float)WIDTH, (float)HEIGHT, pos, target, up);
		Renderer render;
		render.InitObj(teapot);
		camera.setFar(teapot);
		bool run = true;
		bool triggerTexture = false;
		Texture texture;
		texture.loadTexture("resources/test.png");
		SDL_Event e;
		while(run)
		{
			while (SDL_PollEvent(&e))
				event(e, transform, camera, run, triggerTexture);
			glClearColor(0.5f,0.5f,0.5f,1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Matrix<float> model = transform.getModelMatrix();
			Matrix<float> view = camera.buildView();
			Matrix<float> projection = camera.buildProjection();
			Matrix<float> MVP = projection * view * model;
			glUseProgram(render.getShader());
			glUniform1i(glGetUniformLocation(render.getShader(), "useTexture"), triggerTexture ? 1 : 0);
			if(triggerTexture) {
				texture.bind();
			}
			else
				texture.unbind();
			render.renderObj(MVP, teapot, model);
			SDL_GL_SwapWindow(window.getWin());
		}
		render.cleanup(teapot);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return (0);
}