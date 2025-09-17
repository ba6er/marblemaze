#pragma once
#include <la/linalg.hpp>

namespace rn {

	class Camera {

	public:
		Camera();

		void project2d(float left, float right, float bottom, float top, float near, float far);
		void project3d(float fov, float aspect, float near, float far);

		void setPosition(la::Vec3 pos);
		void setTarget(la::Vec3 target);
		void setUp(la::Vec3 up);

		la::Vec3 getPosition() const;
		la::Vec3 getTarget() const;
		la::Vec3 getUp() const;

		la::Mat4 getViewMatrix() const;
		la::Mat4 getProjectionMatrix() const;

	private:
		void updateViewMatrix();

		la::Vec3 pos, target, up;
		la::Mat4 view, projection;
	};
}
