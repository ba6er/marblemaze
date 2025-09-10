#include <geo/geometrytransform.hpp>
#include <gm/game.hpp>
#include <ren/renderer.hpp>
#include <cmath>

using namespace gm;

// TEMP
static float camYaw = lin::Pi / 4, camPitch = lin::Pi / 8, camDistance = 10;
// TEMP

void Game::onInit(int width, int height, ren::RenderAssetManager& ram) {
	ren::Renderer::resizeFrame(width, height);

	lin::Vec3 cameraPos = {
		4.5f + std::cosf(camPitch) * std::sinf(camYaw) * camDistance,
		std::sinf(camPitch) * camDistance,
		4.5f + std::cosf(camPitch) * std::cosf(camYaw) * camDistance,
	};
	currentScene.camera.setPosition(cameraPos);
	currentScene.camera.setTarget({4.5f, 0.0f, 4.5f});
	currentScene.camera.project3d(72 * lin::DegToRad, (float)width / (float)height, 0.001f, 999.9f);

	currentScene.light = {
		{1.0f, 9.9f, 1.0f},
		{0.3f, 0.3f, 0.3f},
		{1.0f, 1.0f, 1.0f},
		{0.5f, 0.5f, 0.5f},
	};

	currentScene.maze.loadFromFile(_RES_PATH "testLevel.txt");

	currentScene.marble.position = {5, 5, 5};
	currentScene.marble.velocity = {-0.5f, -1.5f, 0.2f};

	auto& maze = ram.createMesh("maze");
	maze.create();
	maze.addGeometry(currentScene.maze.toGeometry());

	currentScene.renderables.push_back(ren::Renderable());
	currentScene.renderables[0].create(maze, ram.getMaterial("copperTextured"));

	auto& marble = ram.createMesh("marble");
	marble.create();
	marble.addGeometry(currentScene.marble.toGeometry());

	currentScene.renderables.push_back(ren::Renderable());
	currentScene.renderables[1].create(marble, ram.getMaterial("emerald"));
}

void Game::onResize(int width, int height) {
	ren::Renderer::resizeFrame(width, height);
	currentScene.camera.project3d(72 * lin::DegToRad, (float)width / (float)height, 0.001f, 999.9f);
}

bool Game::onUpdate(float deltaTime, float currentTime, const in::Input& input) {
	if (input.getKey(in::Quit) == in::JustReleased) {
		return false;
	}

	// TEMP
	bool moved = false;
	if (input.getKey(in::MazeYawDecrease) == in::Pressed) {
		camYaw -= lin::Pi * deltaTime;
		moved = true;
	}
	if (input.getKey(in::MazeYawIncrease) == in::Pressed) {
		camYaw += lin::Pi * deltaTime;
		moved = true;
	}
	if (input.getKey(in::MazePitchDecrease) == in::Pressed) {
		camPitch -= lin::Pi * deltaTime;
		if (camPitch < -lin::Pi / 2 + 0.01f) {
			camPitch = -lin::Pi / 2 + 0.01f;
		}
		moved = true;
	}
	if (input.getKey(in::MazePitchIncrease) == in::Pressed) {
		camPitch += lin::Pi * deltaTime;
		if (camPitch > lin::Pi / 2 - 0.01f) {
			camPitch = lin::Pi / 2 - 0.01f;
		}
		moved = true;
	}
	if (input.getKey(in::CameraAngleDecrease) == in::Pressed) {
		camDistance -= deltaTime * 5;
		moved = true;
	}
	if (input.getKey(in::CameraAngleIncrease) == in::Pressed) {
		camDistance += deltaTime * 5;
		moved = true;
	}
	if (moved) {
		lin::Vec3 cameraPos = {
			4.5f + std::cosf(camPitch) * std::sinf(camYaw) * camDistance,
			std::sinf(camPitch) * camDistance,
			4.5f + std::cosf(camPitch) * std::cosf(camYaw) * camDistance,
		};
		currentScene.camera.setPosition(cameraPos);
	}
	// TEMP

	currentScene.updatePhysics(deltaTime);

	return true;
}

void Game::onRender(float deltaTime, float currentTime, ren::RenderAssetManager& ram) {
	currentScene.renderables[1].transform = lin::Mat4::Translate(currentScene.marble.position);

	ren::Renderer::clear(0.5f, 0.5f, 0.5f);
	ren::Renderer::render(currentScene.camera, currentScene.renderables, currentScene.light);
}
