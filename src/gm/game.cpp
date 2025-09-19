#include <gm/game.hpp>
#include <rn/renderer.hpp>
#include <iomanip>
#include <sstream>

using namespace gm;

Game::Game() : state(GameState::ScenePaused), internalSize({0, 0}), gui(), scene() {}

void Game::onInit(int width, int height, float internalWidth, float internalHeight, rs::ResourceManager& resource) {
	rn::Renderer::resizeFrame(width, height);
	internalSize = {internalWidth, internalHeight};

	state = GameState::ScenePaused;

	gui.create(resource.getShader("text"), resource.getFont("noto48"), resource.createMesh("gui", 600));
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
		resource.getSound("select").play();
	}

	if (input.getKey(in::Pause) == in::JustPressed) {
		if (state == GameState::ScenePaused) {
			setState(GameState::ScenePlaying);
		}
		else if (state == GameState::ScenePlaying) {
			setState(GameState::ScenePaused);
		}
		resource.getSound("select").play();
	}

	if (state == GameState::ScenePaused) {
		return true;
	}

	if (scene.checkWinCondition()) {
		std::stringstream timeText;
		timeText << "Time: " << std::setprecision(4) << scene.getTime() << "s";
		gui.addLabel("title").create(48, timeText.str(), {320, 415, 0});
		gui.addLabel("paused").create(48, "Press P to play again", {320, 440, 0});

		setState(GameState::ScenePaused);
		scene.restart();

		resource.getSound("select").play();
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
	return true;
}

void Game::onRender(float deltaTime, float currentTime, rs::ResourceManager& resource) {
	rn::Renderer::clear(0, 0, 0);

	scene.display();
	gui.display();
}

GameState Game::getState() {
	return state;
}

void Game::setState(GameState state) {
	switch (state) {
	case GameState::MenuMain:
		setStateMenuMain();
		break;
	case GameState::MenuOptions:
		setStateMenuOption();
		break;
	case GameState::MenuLevels:
		setStateMenuLevels();
		break;
	case GameState::ScenePlaying:
		setStateScenePlaying();
		break;
	case GameState::ScenePaused:
		setStateScenePaused();
		break;
	}
}

void Game::setStateMenuMain() {
}

void Game::setStateMenuOption() {
}

void Game::setStateMenuLevels() {
}

void Game::setStateScenePlaying() {
	state = GameState::ScenePlaying;
	gui.clear();
}

void Game::setStateScenePaused() {
	state = GameState::ScenePaused;
	gui.addLabel("paused").create(48, "Press P to resume", {320, 440, 0});
}
