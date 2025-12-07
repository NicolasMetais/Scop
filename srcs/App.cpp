#include <App.hpp>

App::App(int width, int height, const std::string& objPath) : window(width, height), renderer(), mesh()
		, camera(static_cast<float>(width), static_cast<float>(height), Vector<float>{0, 0, 5}, Vector<float>{0,0,0}, Vector<float>{0,1,0})
		, transform(), skybox(), timer(), running(true) {

			this->mesh.loadObj(objPath);

			this->renderer.InitObj(this->mesh);
			this->transform.setScale(1.0f);
			this->transform.setPosition(0.0f, 0.0f, 0.0f);

			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glDepthMask(GL_FALSE);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

};

App::~App(){};

void App::processEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		event(e, this->camera, this->running);
		mouse.processEvent(e);
		keyboard.processEvent(e, this->running, this->renderer, this->camera, this->fps);
	}
	mouse.applyRotation(this->transform, this->camera);
	keyboard.applyMovement(this->camera, this->transform, this->deltaTime);
};

void App::update() {
};

void App::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Matrix<float> model = this->transform.getModelMatrix();
	Matrix<float> view = this->camera.buildView();
	Matrix<float> projection = this->camera.buildProjection();
	Matrix<float> MVP = projection * view * model;

	this->renderer.renderObj(MVP, this->mesh, model, this->camera, this->deltaTime);
	this->skybox.draw(this->camera.buildViewNoTranslation(), projection);
	SDL_GL_SwapWindow(this->window.getWin());
};

void App::run(){
	SDL_GL_SetSwapInterval(0);

	while(this->running)
	{
		this->deltaTime = timer.tick();
		processEvents();
		update();
		render();
		FPScalculator();
	}
	this->renderer.cleanup(this->mesh);
};

void App::FPScalculator() {
	this->fps = 1.0f / this->deltaTime;
};
