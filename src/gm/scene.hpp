#pragma once
#include <gm/marble.hpp>
#include <gm/maze.hpp>
#include <rn/camera.hpp>
#include <rn/renderable.hpp>
#include <rn/renderer.hpp>
#include <rs/resourcemanager.hpp>
#include <array>
#include <string_view>
#include <vector>

namespace gm {

	class Scene {

	public:

		Scene();

		bool createFromFile(std::string_view fileName, rs::ResourceManager& resource);
		void createMenuScene(rs::ResourceManager& resource);
		void restart();
		void destroy();

		void setCameraValues(float yaw, float pitch, float distance);
		void updateCamera();
		void updateCamera(float deltaYaw, float deltaPitch, float deltaDistance);
		void updateMazeRotation(float deltaPitch, float deltaRoll);
		void updatePhysics(float deltaTime);
		void updateTimer(float deltaTime);

		std::string_view getId();
		bool checkWinCondition();
		float getTime();

		float getCameraYaw();
		float getCameraPitch();
		float getCameraDistance();

		void setProjection(float fov, float ratio);
		void display();
		bool shouldPlaySound();

	private:
		void marbleBlockCollision(float x, float y, float z, float deltaTime);

		static float DistanceSphereAABB(la::Vec3 box, la::Vec3 sphere);

		std::string id;
		float timer;

		rn::Camera camera;
		rn::Light light;
		std::vector<rn::Renderable> renderables;

		Marble marble;
		Maze maze;
		la::Vec3 start, finish, initCameraValues;

		float cameraDistance, cameraYaw, cameraPitch;

		std::array<bool, 3> marbleIsTouchingWalls, marbleWasTouchingWalls;
	};
}
