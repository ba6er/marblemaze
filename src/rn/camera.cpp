#include <rn/camera.hpp>

using namespace rn;

Camera::Camera() : pos({0, 0, -1}), target({0, 0, 0}), up({0, 1, 0}), projection(la::Mat4::Identity()) {
	updateViewMatrix();
}

void Camera::project2d(float left, float right, float bottom, float top, float near, float far) {
	projection = la::Mat4::Project2d(left, right, bottom, top, near, far);
}

void Camera::project3d(float fov, float aspect, float near, float far) {
	projection = la::Mat4::Project3d(fov, aspect, near, far);
}

void Camera::setPosition(la::Vec3 pos) {
	this->pos = pos;
	updateViewMatrix();
}

void Camera::setTarget(la::Vec3 target) {
	this->target = target;
	updateViewMatrix();
}

la::Vec3 Camera::getPosition() const {
	return pos;
}

la::Vec3 Camera::getTarget() const {
	return target;
}

la::Vec3 Camera::getUp() const {
	return up;
}

void Camera::setUp(la::Vec3 up) {
	this->up = up;
	updateViewMatrix();
}

la::Mat4 Camera::getViewMatrix() const {
	return view;
}

la::Mat4 Camera::getProjectionMatrix() const {
	return projection;
}

void Camera::updateViewMatrix() {
	view = la::Mat4::LookAt(pos, target, up);
}
