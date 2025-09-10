#include <geo/geometrytransform.hpp>
#include <gm/game.hpp>
#include <ren/renderer.hpp>
#include <cmath>

using namespace gm;

// TEMP
static float camYaw = lin::Pi / 4, camPitch = lin::Pi / 8, camDistance = 5.0f;
// TEMP

void Game::onInit(int width, int height, ren::RenderAssetManager& ram) {
	ren::Renderer::resizeFrame(width, height);

	lin::Vec3 cameraPos = {
		std::cosf(camPitch) * std::sinf(camYaw) * camDistance,
		std::sinf(camPitch) * camDistance,
		std::cosf(camPitch) * std::cosf(camYaw) * camDistance,
	};
	currentScene.camera.setPosition(cameraPos);
	currentScene.camera.project3d(72 * lin::DegToRad, (float)width / (float)height, 0.001f, 999.9f);

	currentScene.light = {
		{1.0f, 9.9f, 1.0f},
		{0.3f, 0.3f, 0.3f},
		{1.0f, 1.0f, 1.0f},
		{0.5f, 0.5f, 0.5f},
	};

	currentScene.maze.loadFromFile(_RES_PATH "testLevel.txt");

	auto cubeData = currentScene.maze.toGeometry();
	geo::GeometryTransform::Translate(cubeData, {-4.5f, -1.0f, -4.5f});
	geo::GeometryTransform::Scale(cubeData, {0.4f, 0.4f, 0.4f});
	auto& cube = ram.createMesh("cube");
	cube.create();
	cube.addGeometry(cubeData);

	currentScene.renderables.push_back(ren::Renderable());
	currentScene.renderables[0].create(ram.getMesh("cube"), ram.getMaterial("copper"));
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
			std::cosf(camPitch) * std::sinf(camYaw) * camDistance,
			std::sinf(camPitch) * camDistance,
			std::cosf(camPitch) * std::cosf(camYaw) * camDistance,
		};
		currentScene.camera.setPosition(cameraPos);
	}
	// TEMP

	return true;
}

void Game::onRender(float deltaTime, float currentTime, ren::RenderAssetManager& ram) {
	ren::Renderer::clear(0.5f, 0.5f, 0.5f);
	ren::Renderer::render(currentScene.camera, currentScene.renderables, currentScene.light);
}
