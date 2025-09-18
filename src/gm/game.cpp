#include <gm/game.hpp>
#include <rn/renderer.hpp>

using namespace gm;

Game::Game() : paused(true), internalSize({0, 0}), gui(), scene() {}

void Game::onInit(int width, int height, float internalWidth, float internalHeight, rs::ResourceManager& resource) {
	rn::Renderer::resizeFrame(width, height);
	internalSize = {internalWidth, internalHeight};

	paused = true;
	gui.create(resource.getShader("text"), resource.getFont("noto48"), resource.createMesh("gui"));
	gui.setFrame(0, width, 0, height);
	gui.addLabel("title").create(96, "Marble Maze", {320, 80, 0});
	gui.addLabel("paused").create(48, "Press P to play", {320, 440, 0});

	bool success = scene.createFromFile(_RES_PATH "testLevel.txt", resource);
	if (!success) {
		DEBUG_ERROR("Failed to load test maze");
	}
	scene.setProjection(72 * la::DegToRad, (float)width / (float)height);
}

void Game::onResize(int width, int height) {
	rn::Renderer::resizeFrame(width, height);
	scene.setProjection(72 * la::DegToRad, (float)width / (float)height);

	float ox = (internalSize.x - width * internalSize.y / height) / 2;
	gui.setFrame(ox, internalSize.x - ox, 0, internalSize.y);
}

bool Game::onUpdate(float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input) {
	if (input.getKey(in::Quit) == in::JustReleased) {
		return false;
	}

	if (input.getKey(in::Restart) == in::JustPressed) {
		scene.restart();
	}

	if (input.getKey(in::Pause) == in::JustPressed) {
		togglePause();
		if (paused) {
			gui.addLabel("paused").create(48, "Press P to resume", {320, 440, 0});
		} else {
			gui.removeLabel("title");
			gui.removeLabel("paused");
		}
	}

	if (paused) {
		return true;
	}

	// Update the scene when not paused
	int pressed[] = {
		input.getKey(in::MazeYawDecrease) == in::Pressed,
		input.getKey(in::MazeYawIncrease) == in::Pressed,
		input.getKey(in::MazeRollDecrease) == in::Pressed,
		input.getKey(in::MazeRollIncrease) == in::Pressed,
	};

	if (pressed[3] || pressed[2] || pressed[1] || pressed[0]) {
		scene.updateMazeRotation(
			(pressed[1] - pressed[0]) * deltaTime,
			(pressed[3] - pressed[2]) * deltaTime);
	}

	if (input.getMouseL() == in::Pressed || input.getScrollY() != 0) {
		scene.updateCamera(
			input.getDeltaMouseX() * deltaTime * 0.2f,
			input.getDeltaMouseY() * deltaTime * 0.2f,
			-input.getScrollY() * deltaTime * 10);
	}

	scene.updatePhysics(deltaTime);
	if (scene.shouldPlaySound()) {
		resource.getSound("solid").play();
	}
	scene.checkWinCondition();

	return true;
}

void Game::onRender(float deltaTime, float currentTime, rs::ResourceManager& resource) {
	rn::Renderer::clear(0, 0, 0);

	scene.display();
	gui.display();
}

bool Game::togglePause() {
	paused = !paused;
	return paused;
}

bool Game::isPaused() {
	return paused;
}
