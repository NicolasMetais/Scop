#include <Camera.hpp>

Camera::Camera(float w, float h, Math::Vec3 pos) {
	this->cameraPos = pos;
	// U = Math::Vec3(1.0f, 0.0f, 0.0f);
	// V = Math::Vec3(0.0f, 1.0f, 0.0f);
	// N = Math::Vec3(0.0f, 0.0f, 1.0f);
	this->target = Math::Vec3(0.0f, 0.0f, 1.0f);
	this->up = Math::Vec3(0.0f, 1.0f, 0.0f);
	this->fov = 45.0f * (3.1415926f / 180.0f);
	this->aspect = w / h;
	this->speed = 0.1f;
};

Camera::Camera(float w, float h, Math::Vec3& pos, Math::Vec3& target, Math::Vec3& up) {
	this->winWidth = w;
	this->winHeight = h;
	this->cameraPos = pos;
	this->target = target;
	this->target.normalize();
	this->up = up;
	this->up.normalize();
	this->fov = 45.0f * (3.1415926f / 180.0f);
	this->aspect = w / h;
	this->speed = 0.1f;
	cameraInit();
};

void Camera::cameraInit() {
	Math::Vec3 HTarget(target.x, 0.0, target.z);
	HTarget.normalize();
	float angle = Math::Todegres(asin(abs(HTarget.z)));

	if (HTarget.z >= 0.0f)
	{
		if (HTarget.x >= 0.0f)
			this->angleH = 360.0f - angle;
		else
			this->angleH = 180.0f + angle;
	}
	else
	{
		if (HTarget.x >= 0.0f)
			this->angleH = angle;
		else
			this->angleH = 180.0f - angle;
	}
	this->angleV = -Math::Todegres(asin(target.y));
	this->upperEdge = false;
	this->leftEdge = false;
	this->rightEdge = false;
	this->lowerEdge = false;
	mousePos.x = winWidth / 2;
	mousePos.y = winHeight / 2;
};

void Camera::mouseActions() {
	int DeltaX = 0.0f;
	int DeltaY = 0.0f;
	SDL_GetRelativeMouseState(&DeltaX, &DeltaY);
	this->angleH += DeltaX * 0.1f;
	this->angleV += DeltaY * 0.1f;
	if (this->angleV > 89.0f) angleV = 89.0f;
	if (this->angleV < -89.0f) angleV = -89.0f;
	cameraUpdate();
};

void Camera::cameraUpdate() {
	Math::Vec3 Yaxis(0.0f, 1.0f, 0.0f);

	Math::Vec3 View(1.0f, 0.0f, 0.0f);

	View.Rotate(this->angleH, Yaxis);
	View.normalize();

	Math::Vec3 U = Yaxis.cross(View);
	U.normalize();

	View.Rotate(this->angleV, U);
	View.normalize();

	target = View;
	target.normalize();
	
	up = target.cross(U);
	up.normalize();
};

void Camera::setFar(const ObjModel& obj) {
	float camZ = obj.getMaxVertices().z + (obj.getRadius() * 3.0f);
	far = camZ + obj.getRadius() * 4.0f;
	near = 0.2f;
	far = 100.0f;
};


Math::Matrix4f Camera::buildProjection() {
	return Math::Matrix4f::perspective(fov, aspect, near, far);
};

Math::Matrix4f Camera::updateProjection(float w, float h) {
	aspect = w / h;
	return Math::Matrix4f::perspective(fov, aspect, near, far);
};


Math::Matrix4f Camera::buildView() {
	N = target.normalize();
	U = up.cross(N).normalize();
	V = N.cross(U);
	return Math::Matrix4f::view(cameraPos, U, V, N);
};

void Camera::moveUp() {
	this->cameraPos.y += this->speed;
}

void Camera::moveDown() {
	this->cameraPos.y -= this->speed;
}

void Camera::moveForward() {
	this->cameraPos -= (target * this->speed);
}

void Camera::moveBackward() {
	this->cameraPos += (target * this->speed);
}

void Camera::moveLeft() {
	Math::Vec3 Right = target.cross(up);
	Right.normalize();
	this->cameraPos += Right * this->speed;
}

void Camera::moveRight() {
	Math::Vec3 Right = target.cross(up);
	Right.normalize();
	this->cameraPos -= Right * this->speed;
}

void Camera::speedUp() {
	this->speed += 0.1f;
	std::cout << "Speed changed to " << speed << std::endl;
}

void Camera::speedDown() {
	this->speed -= 0.1f;
	if (this->speed < 0.1f)
		this->speed = 0.1f;
	std::cout << "Speed changed to " << speed << std::endl;
}