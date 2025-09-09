#include <geo/geometrygenerator.hpp>
#include <gm/game.hpp>
#include <ren/renderer.hpp>
#include <cmath>

using namespace gm;

void Game::onInit(int width, int height, ren::RenderAssetManager& ram) {
	ren::Renderer::resizeFrame(width, height);

	auto cubeData = geo::GeometryGenerator::GenerateCube();
	auto& cube = ram.createMesh("cube");
	cube.create();
	cube.addGeometry(cubeData);

	currentScene.renderables.push_back(ren::Renderable());
	currentScene.renderables[0].create(ram.getMesh("cube"), ram.getMaterial("copper"));

	currentScene.camera.project3d(72 * lin::DegToRad, (float)width / (float)height, 0.001f, 999.9f);
	currentScene.camera.setPosition({0, 0, 5});
	currentScene.camera.setTarget({0, 0, 0});
	currentScene.camera.setUp({0, 1, 0});
}

void Game::onResize(int width, int height) {
	ren::Renderer::resizeFrame(width, height);
	currentScene.camera.project3d(72 * lin::DegToRad, (float)width / (float)height, 0.001f, 999.9f);
}

bool Game::onUpdate(float deltaTime, float currentTime, const in::Input& input) {
	if (input.getKey(in::Quit) == in::JustReleased) {
		return false;
	}
	return true;
}

void Game::onRender(float deltaTime, float currentTime, ren::RenderAssetManager& ram) {
	lin::Vec3 cameraPos = {
		std::sinf(currentTime / 2) * 5,
		std::sinf(currentTime) * 2,
		std::cosf(currentTime / 2) * 5,
	};
	currentScene.camera.setPosition(cameraPos);

	ren::Renderer::clear(0.0f, 0.2f, 0.3f);
	ren::Renderer::render(currentScene.camera, currentScene.renderables);
}
