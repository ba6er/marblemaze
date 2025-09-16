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
		Scene();

		void updateCamera();
		void updateCamera(float deltaYaw, float deltaPitch, float deltaDistance);
		void updateMazeRotation(float deltaPitch, float deltaRoll);
		void updatePhysics(float deltaTime);

		ren::Camera camera;
		ren::Light light;
		std::vector<ren::Renderable> renderables;

		Marble marble;
		Maze maze;

		float cameraDistance, cameraYaw, cameraPitch;

	private:
		void marbleBlockCollision(float x, float y, float z, float deltaTime);

		static float DistanceSphereAABB(lin::Vec3 box, lin::Vec3 sphere);
	};
}
