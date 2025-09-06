#include <gm/game.hpp>
#include <ren/renderer.hpp>

using namespace gm;

void Game::onInit(ren::RenderAssetManager& ram) {
	currentScene.renderables.push_back(ren::Renderable());
	currentScene.renderables[0].create(ram.getMesh("triangle"), ram.getMaterial("copper"));

	currentScene.camera.project3d(72 * lin::degToRad, 1.33f, 0.001f, 999.9f);
	currentScene.camera.setPosition({0, 0, -10});
	currentScene.camera.setTarget({0, 0, 0});
	currentScene.camera.setUp({0, 1, 0});
}

bool Game::onUpdate(float deltaTime, float currentTime, const in::Input& input) {
	if (input.getKey(in::Quit) == in::JustReleased) {
		return false;
	}
	return true;
}

void Game::onRender(float deltaTime, float currentTime, ren::RenderAssetManager& ram) {
	ren::Renderer::clear(0.0f, 0.2f, 0.3f);
	ren::Renderer::render(currentScene.camera, currentScene.renderables);
}
