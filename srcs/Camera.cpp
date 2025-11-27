#include <Camera.hpp>
#include <Transform.hpp>
#include <Matrix/Matrix.hpp>
#include <utils.hpp>

Camera::Camera(float w, float h, Vector<float> pos) : cameraPos(2), target(3), up(3), U(3), V(3), N(3), mousePos(2) {
	this->cameraPos = pos;
	// U = Math::Vec3(1.0f, 0.0f, 0.0f);
	// V = Math::Vec3(0.0f, 1.0f, 0.0f);
	// N = Math::Vec3(0.0f, 0.0f, 1.0f);
	this->target = Vector<float>{0.0f, 0.0f, 1.0f};
	this->up = Vector<float>{0.0f, 1.0f, 0.0f};
	this->fov = 45.0f * (3.1415926f / 180.0f);
	this->aspect = w / h;
	this->speed = 0.1f;
};

Camera::Camera(float w, float h, Vector<float>& pos, Vector<float>& target,Vector<float>& up) : cameraPos(2), target(3), up(3), U(3), V(3), N(3), mousePos(2){
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
	Vector<float> HTarget{target.x(), 0.0, target.z()};
	HTarget.normalize();
	float angle = utils::Todegres(asin(abs(HTarget.z())));
	if (HTarget.z() >= 0.0f)
	{
		if (HTarget.x() >= 0.0f)
		{
			this->angleH = 360.0f - angle;
		}
		else
			this->angleH = 180.0f + angle;
	}
	else
	{
		if (HTarget.x() >= 0.0f)
			this->angleH = angle;
		else
			this->angleH = 180.0f - angle;
	}
	this->angleV = -utils::Todegres(asin(target.y()));
	this->upperEdge = false;
	this->leftEdge = false;
	this->rightEdge = false;
	this->lowerEdge = false;
	mousePos.x() = winWidth / 2;
	mousePos.y() = winHeight / 2;
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
	Vector<float> forward{0.0f, 0.0f, -1.0f};
	Vector<float> Yaxis{0.0f, 1.0f, 0.0f};

	Quaternion<float> qH(angleH, Yaxis);
	forward = qH * forward;
	Vector<float> right = cross_product(forward, Yaxis).normalize();

    Quaternion<float> qV(angleV, right);
    forward = qV * forward;
    forward.normalize();

    target = forward;

	N = target * -1.0f;
    N = N.normalize();
    U = cross_product(Yaxis, N).normalize();
    V = cross_product(N, U);
};

void Camera::setFar(const Mesh& obj) {
	float camZ = obj.getMaxVertices().z() + (obj.getRadius() * 3.0f);
	far = camZ + obj.getRadius() * 4.0f;
	near = 0.2f;
	far = 100.0f;
};

Matrix<float> Camera::buildProjection() {
	return utils::perspective(fov, aspect, near, far);
};

Matrix<float> Camera::updateProjection(float w, float h) {
	aspect = w / h;
	return utils::perspective(fov, aspect, near, far);
};


Matrix<float> Camera::buildView() {
	// N = target.normalize();
	// U = cross_product(up, N).normalize();
	// V = cross_product(N, U);
	return utils::view(cameraPos, U, V, N);
};

void Camera::moveUp() {
	this->cameraPos.y() += this->speed;
}

void Camera::moveDown() {
	this->cameraPos.y() -= this->speed;
}

void Camera::moveForward() {
	this->cameraPos += (target * this->speed);
}

void Camera::moveBackward() {
	this->cameraPos -= (target * this->speed);
}

void Camera::moveLeft() {
	Vector<float> Right = cross_product(target, up);
	Right.normalize();
	this->cameraPos -= Right * this->speed;
}

void Camera::moveRight() {
	Vector<float> Right = cross_product(target, up);
	Right.normalize();
	this->cameraPos += Right * this->speed;
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