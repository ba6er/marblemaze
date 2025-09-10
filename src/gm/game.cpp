#include <geo/geometrygenerator.hpp>
#include <geo/geometrytransform.hpp>
#include <gm/game.hpp>
#include <ren/renderer.hpp>
#include <cmath>

using namespace gm;

// TEMP
static float camYaw = lin::Pi / 4, camPitch = lin::Pi / 8, cubeAngle = 0.0f;
// TEMP

void Game::onInit(int width, int height, ren::RenderAssetManager& ram) {
	ren::Renderer::resizeFrame(width, height);

	auto cubeData = geo::GeometryGenerator::GenerateCube();
	auto& cube = ram.createMesh("cube");
	geo::GeometryTransform::Scale(cubeData, {2, 2, 2});

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
		currentScene.renderables[0].material->setUniform("uViewPosition", currentScene.camera.getPosition());
	}

	if (input.getKey(in::CameraAngleDecrease) == in::Pressed) {
		cubeAngle -= lin::Pi * deltaTime;
		currentScene.renderables[0].transform = lin::Mat4::Rotate(cubeAngle, {0, 1, 0});
	}
	if (input.getKey(in::CameraAngleIncrease) == in::Pressed) {
		cubeAngle += lin::Pi * deltaTime;
		currentScene.renderables[0].transform = lin::Mat4::Rotate(cubeAngle, {0, 1, 0});
	}
	// TEMP

	return true;
}

void Game::onRender(float deltaTime, float currentTime, ren::RenderAssetManager& ram) {
	ren::Renderer::clear(0.5f, 0.5f, 0.5f);
	ren::Renderer::render(currentScene.camera, currentScene.renderables);
}
