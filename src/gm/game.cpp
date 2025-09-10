#include <geo/geometrygenerator.hpp>
#include <geo/geometrytransform.hpp>
#include <gm/game.hpp>
#include <ren/renderer.hpp>
#include <cmath>

using namespace gm;

// TEMP
static float camYaw = lin::Pi / 4, camPitch = lin::Pi / 8;
// TEMP

void Game::onInit(int width, int height, ren::RenderAssetManager& ram) {
	ren::Renderer::resizeFrame(width, height);

	auto cubeData = geo::GeometryGenerator::GenerateCube();
	auto& cube = ram.createMesh("cube");
	geo::GeometryTransform::Scale(cubeData, {0.5f, 3.0f, 2.0f});
	geo::GeometryTransform::ApplyColor(cubeData, {0, 1, 1});

	cube.create();
	cube.addGeometry(cubeData);

	currentScene.renderables.push_back(ren::Renderable());
	currentScene.renderables[0].create(ram.getMesh("cube"), ram.getMaterial("copper"));

	lin::Vec3 cameraPos = {
		std::sinf(camYaw) * 5,
		std::sinf(camPitch) * 2,
		std::cosf(camYaw) * 5,
	};
	currentScene.camera.setPosition(cameraPos);
	currentScene.camera.project3d(72 * lin::DegToRad, (float)width / (float)height, 0.001f, 999.9f);
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
		if (camPitch < -lin::Pi / 2 + 0.1f) {
			camPitch = -lin::Pi / 2 + 0.1f;
		}
		moved = true;
	}
	if (input.getKey(in::MazePitchIncrease) == in::Pressed) {
		camPitch += lin::Pi * deltaTime;
		if (camPitch > lin::Pi / 2 - 0.1f) {
			camPitch = lin::Pi / 2 - 0.1f;
		}
		moved = true;
	}
	if (moved) {
		lin::Vec3 cameraPos = {
			std::sinf(camYaw) * 5,
			std::sinf(camPitch) * 2,
			std::cosf(camYaw) * 5,
		};
		currentScene.camera.setPosition(cameraPos);
	}
	// TEMP

	return true;
}

void Game::onRender(float deltaTime, float currentTime, ren::RenderAssetManager& ram) {
	currentScene.renderables[0].transform = lin::Mat4::Rotate(currentTime, {0, 1, 1});
	ren::Renderer::clear(0.3f, 0.1f, 0.1f);
	ren::Renderer::render(currentScene.camera, currentScene.renderables);
}
