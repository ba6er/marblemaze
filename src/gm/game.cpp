#include <gm/game.hpp>

using namespace gm;

bool Game::onUpdate(float deltaTime, float currentTime, const in::Input& input) {
	if (input.getKey(in::Quit) == in::JustReleased) {
		return false;
	}
	return true;
}

void Game::onRender(float deltaTime, float currentTime) {
}
