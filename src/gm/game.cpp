#include <gm/game.hpp>
#include <rn/renderer.hpp>
#include <iomanip>
#include <sstream>

using namespace gm;

Game::Game() : state(MenuMain), frameSize({0, 0}), internalSize({0, 0}), gui(), scene() {}

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

	if (state & InScene) {
		scene.setProjection(72 * la::DegToRad, frameSize.x / frameSize.y);
	}
}

bool Game::onUpdate(float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input) {
	if (input.getKey(in::Quit) == in::JustReleased) {
		return false;
	}

	if (state == MenuMain) {
		gui.checkButton("title", input.getMouseX(), input.getMouseY());

		if (input.getKey(in::MenuSelect) != in::JustPressed) {
			return true;
		}
		if (scene.createFromFile(_RES_PATH "testLevel.txt", resource) == true) {
			scene.setProjection(72 * la::DegToRad, frameSize.x / frameSize.y);
			setState(ScenePlaying);
		} else {
			DEBUG_ERROR("Failed to load test maze");
		}
		resource.getSound("select").play();
		return true;
	}
	else if (state == MenuOptions) {
		return true;
	}
	else if (state == MenuLevels) {
		return true;
	}

	// Gameplay

	if (input.getKey(in::GameRestart) == in::JustPressed) {
		scene.restart();
		resource.getSound("select").play();
	}

	if (input.getKey(in::GamePause) == in::JustPressed) {
		if (state == ScenePaused) {
			setState(ScenePlaying);
		}
		else if (state == ScenePlaying) {
			setState(ScenePaused);
		}
		resource.getSound("select").play();
	}

	if (state == ScenePaused || state == SceneWin) {
		return true;
	}

	if (scene.checkWinCondition()) {
		setState(SceneWin);
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
	if (state & InMenu) {
		rn::Renderer::clear(0.2f, 0.2f, 0.3f);
	} else {
		rn::Renderer::clear(0.0f, 0.0f, 0.0f);
	}

	if (state & InScene) {
		scene.display();
	}
	gui.display();
}

GameState Game::getState() {
	return state;
}

void Game::setState(GameState state) {
	switch (state) {
	case MenuMain:
		DEBUG_TRACE("State MenuMain");
		setStateMenuMain();
		break;
	case MenuOptions:
		DEBUG_TRACE("State MenuOptions");
		setStateMenuOption();
		break;
	case MenuLevels:
		DEBUG_TRACE("State MenuLevels");
		setStateMenuLevels();
		break;
	case ScenePlaying:
		DEBUG_TRACE("State ScenePlaying");
		setStateScenePlaying();
		break;
	case ScenePaused:
		DEBUG_TRACE("State ScenePaused");
		setStateScenePaused();
		break;
	case SceneWin:
		DEBUG_TRACE("State SceneWin");
		setStateSceneWin();
		break;
	default:
		DEBUG_WARNING("Invalid state %d, not changing", state);
		break;
	}
}

void Game::setStateMenuMain() {
	state = MenuMain;

	gui.addButton("title", 96, "Marble Maze", {320, 80});
	gui.addLabel("play", 48, "Press ENTER to play", {320, 440});
}

void Game::setStateMenuOption() {
	state = MenuOptions;
}

void Game::setStateMenuLevels() {
	state = MenuLevels;
}

void Game::setStateScenePlaying() {
	state = ScenePlaying;

	gui.clear();
}

void Game::setStateScenePaused() {
	state = ScenePaused;
	gui.addLabel("paused", 48, "Press P to resume", {320, 440});
}

void Game::setStateSceneWin() {
	state = SceneWin;

	std::stringstream timeText;
	timeText << "Time: " << std::setprecision(4) << scene.getTime() << "s";

	gui.addLabel("win", 48, timeText.str(), {320, 415});
	gui.addLabel("play", 48, "Press P to play again", {320, 440});

	scene.restart();
}
