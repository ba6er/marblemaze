#pragma once
#include <gm/marble.hpp>
#include <gm/maze.hpp>
#include <ren/camera.hpp>
#include <ren/renderable.hpp>
#include <ren/renderer.hpp>
#include <vector>

namespace gm {

	class Scene {

	public:
		void updatePhysics(float deltaTime);

		ren::Camera camera;
		ren::Light light;
		std::vector<ren::Renderable> renderables;

		Marble marble;
		Maze maze;

		float mazeYaw, mazePitch, cameraAngle;

	private:
		void marbleBlockCollision(float x, float y, float z, float deltaTime);

		static float DistanceSphereAABB(lin::Vec3 box, lin::Vec3 sphere);
	};
}
