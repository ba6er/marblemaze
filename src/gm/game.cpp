#include <gm/game.hpp>

using namespace gm;

void Game::onInit(ren::RenderAssetManager& ram) {
}


bool Game::onUpdate(float deltaTime, float currentTime, const in::Input& input) {
	if (input.getKey(in::Quit) == in::JustReleased) {
		return false;
	}
	return true;
}

void Game::onRender(float deltaTime, float currentTime, ren::RenderAssetManager& ram) {
}
