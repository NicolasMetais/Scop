#include <Event.hpp>
#define SENSITIVITY 0.01f

void event(SDL_Event& e, Transform& transform, Camera& camera, bool& run, bool& triggerTexture) {
	static bool rotate = false;
	static float rotX = 0.0f;
	static float rotY = 0.0f;
	static float scale = transform.getScale();
	static int lastX = 0, lastY = 0;
	static int y;
	switch (e.type) {
		case SDL_QUIT:
			run = false;
		break;
		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT) {
				rotate = true;
			lastX = e.button.x;
			lastY = e.button.y;
		}
		break ;
		case SDL_MOUSEBUTTONUP:
			if (e.button.button == SDL_BUTTON_LEFT)
				rotate = false;
		break ;
		case SDL_MOUSEMOTION:
			if (rotate) {
				int deltaX = e.motion.x - lastX;
				int deltaY = e.motion.y - lastY;
				lastX = e.motion.x;
				lastY = e.motion.y;
				rotX += deltaX * SENSITIVITY;
				rotY += deltaY * SENSITIVITY;
				transform.setRotate(rotY, rotX, 0.0f);
			}
			else {
				camera.mouseActions();
			}
		break ;
		case SDL_MOUSEWHEEL:
			y = e.wheel.y;
			if (e.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
				y = -y;
			if (y > 0)
				scale += 0.1f;
			if (y < 0 && scale > 0.1f)
				scale -= 0.1f;
			if (scale < 0.1f)
				scale = 0.1f;
			if (scale > 8.0f)
				scale = 8.0f;
			transform.setScale(scale);
		break ;
		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				int w = e.window.data1;
				int h = e.window.data2;
				glViewport(0, 0, w, h);
				camera.updateProjection(w, h);
			}
			break ;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_t)
				triggerTexture = !triggerTexture;
			if (e.key.keysym.sym == SDLK_ESCAPE)
				run = false;
			if (e.key.keysym.sym == SDLK_w)
				camera.moveForward();
			if (e.key.keysym.sym == SDLK_a)
				camera.moveLeft();
			if (e.key.keysym.sym == SDLK_s)
				camera.moveBackward();
			if (e.key.keysym.sym == SDLK_d)
				camera.moveRight();
			if (e.key.keysym.sym == SDLK_LSHIFT)
				camera.moveUp();
			if (e.key.keysym.sym == SDLK_LCTRL)
				camera.moveDown();
			if (e.key.keysym.sym == SDLK_KP_PLUS || e.key.keysym.sym == SDLK_PLUS)
				camera.speedUp();
			if (e.key.keysym.sym == SDLK_KP_MINUS || e.key.keysym.sym == SDLK_MINUS)
				camera.speedDown();
			break ;
	}
}