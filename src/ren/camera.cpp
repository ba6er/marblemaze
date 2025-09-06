#include <ren/camera.hpp>

using namespace ren;

Camera::Camera() : pos({0, 0, -1}), target({0, 0, 0}), up({0, 1, 0}), projection(lin::Mat4::Identity()) {
	updateViewMatrix();
}


void Camera::project2d(float left, float right, float bottom, float top, float near, float far) {
	projection = lin::Mat4::Project2d(left, right, bottom, top, near, far);
}

void Camera::project3d(float fov, float aspect, float near, float far) {
	projection = lin::Mat4::Project3d(fov, aspect, near, far);
}

void Camera::setPosition(lin::Vec3 pos) {
	this->pos = pos;
	updateViewMatrix();
}

void Camera::setTarget(lin::Vec3 target) {
	this->target = target;
	updateViewMatrix();
}

void Camera::setUp(lin::Vec3 up) {
	this->up = up;
	updateViewMatrix();
}

lin::Mat4 Camera::getViewMatrix() {
	return view;
}

lin::Mat4 Camera::getProjectionMatrix() {
	return projection;
}

void Camera::updateViewMatrix() {
	view = lin::Mat4::LookAt(pos, target, up);
}
