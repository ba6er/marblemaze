#include <gm/game.hpp>
#include <rn/renderer.hpp>
#include <iomanip>
#include <sstream>

using namespace gm;

Game::Game() : state(GameState::ScenePaused), frameSize({0, 0}), internalSize({0, 0}), gui(), scene() {}

void Game::onInit(int width, int height, float internalWidth, float internalHeight, rs::ResourceManager& resource) {
	rn::Renderer::resizeFrame(width, height);
	frameSize = {(float)width, (float)height};
	internalSize = {internalWidth, internalHeight};

	gui.create(resource.getShader("text"), resource.getFont("noto48"), resource.createMesh("gui", 600));
	gui.setFrame(0, width, 0, height);

	setStateMenuMain();
}

void Game::onResize(int width, int height) {
	frameSize = {(float)width, (float)height};
	rn::Renderer::resizeFrame(width, height);

	float ox = (internalSize.x - width * internalSize.y / height) / 2;
	gui.setFrame(ox, internalSize.x - ox, 0, internalSize.y);

	if (state == GameState::ScenePlaying || state == GameState::ScenePaused) {
		scene.setProjection(72 * la::DegToRad, frameSize.x / frameSize.y);
	}
}

bool Game::onUpdate(float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input) {
	if (input.getKey(in::Quit) == in::JustReleased) {
		return false;
	}

	if (state == GameState::MenuMain) {
		if (input.getKey(in::MenuSelect) != in::JustPressed) {
			return true;
		}
		if (scene.createFromFile(_RES_PATH "testLevel.txt", resource) == true) {
			scene.setProjection(72 * la::DegToRad, frameSize.x / frameSize.y);
			setState(GameState::ScenePlaying);
		} else {
			DEBUG_ERROR("Failed to load test maze");
		}
		resource.getSound("select").play();
		return true;
	}
	else if (state == GameState::MenuOptions) {
		return true;
	}
	else if (state == GameState::MenuLevels) {
		return true;
	}

	// Gameplay

	if (input.getKey(in::GameRestart) == in::JustPressed) {
		scene.restart();
		resource.getSound("select").play();
	}

	if (input.getKey(in::GamePause) == in::JustPressed) {
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
	state = GameState::MenuMain;

	gui.addLabel("title").create(96, "Marble Maze", {320, 80, 0});
	gui.addLabel("paused").create(48, "Press ENTER to play", {320, 440, 0});
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
