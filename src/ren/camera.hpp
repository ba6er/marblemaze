#pragma once
#include <lin/linalg.hpp>

namespace ren {

	class Camera {

	public:
		Camera();

		void project2d(float left, float right, float bottom, float top, float near, float far);
		void project3d(float fov, float aspect, float near, float far);

		void setPosition(lin::Vec3 pos);
		void setTarget(lin::Vec3 target);
		void setUp(lin::Vec3 up);

		lin::Vec3 getPosition() const;
		lin::Vec3 getTarget() const;
		lin::Vec3 getUp() const;

		lin::Mat4 getViewMatrix() const;
		lin::Mat4 getProjectionMatrix() const;

	private:
		void updateViewMatrix();

		lin::Vec3 pos, target, up;
		lin::Mat4 view, projection;
	};
}
