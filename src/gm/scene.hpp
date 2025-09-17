#pragma once
#include <gm/marble.hpp>
#include <gm/maze.hpp>
#include <rn/camera.hpp>
#include <rn/renderable.hpp>
#include <rn/renderer.hpp>
#include <vector>

namespace gm {

	class Scene {

	public:
		Scene();

		void updateCamera();
		void updateCamera(float deltaYaw, float deltaPitch, float deltaDistance);
		void updateMazeRotation(float deltaPitch, float deltaRoll);
		void updatePhysics(float deltaTime);

		rn::Camera camera;
		rn::Light light;
		std::vector<rn::Renderable> renderables;

		Marble marble;
		Maze maze;

		float cameraDistance, cameraYaw, cameraPitch;

	private:
		void marbleBlockCollision(float x, float y, float z, float deltaTime);

		static float DistanceSphereAABB(la::Vec3 box, la::Vec3 sphere);
	};
}
