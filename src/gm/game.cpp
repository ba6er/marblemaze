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

	scene.createMenuScene(resource);
	scene.setProjection(72 * la::DegToRad, frameSize.x / frameSize.y);

	setState(MenuMain);
}

void Game::onResize(int width, int height) {
	frameSize = {(float)width, (float)height};
	rn::Renderer::resizeFrame(width, height);

	float ox = (internalSize.x - width * internalSize.y / height) / 2;
	gui.setFrame(ox, internalSize.x - ox, 0, internalSize.y);

	scene.setProjection(72 * la::DegToRad, frameSize.x / frameSize.y);
}

bool Game::onUpdate(float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input) {
	if (input.getKey(in::Quit) == in::JustReleased) {
		return false;
	}

	if (state == MenuMain) {
		return onStateMenuMain(deltaTime, currentTime, resource, input);
	}
	else if (state == MenuOptions) {
		return onStateMenuOptions(deltaTime, currentTime, resource, input);
	}
	else if (state == MenuLevels) {
		return onStateMenuLevels(deltaTime, currentTime, resource, input);
	}
	else if (state == ScenePlaying) {
		return onStateScenePlaying(deltaTime, currentTime, resource, input);
	}
	else if (state == ScenePaused) {
		return onStateScenePaused(deltaTime, currentTime, resource, input);
	}
	else if (state == SceneWin) {
		return onStateSceneWin(deltaTime, currentTime, resource, input);
	}
	DEBUG_WARNING("State %d is invalid", state);
	return true;
}

void Game::onRender(float deltaTime, float currentTime, rs::ResourceManager& resource) {
	if (state & InMenu) {
		rn::Renderer::clear(0.2f, 0.2f, 0.3f);
	} else {
		rn::Renderer::clear(0.0f, 0.0f, 0.0f);
	}

	scene.display();
	gui.display();
}

GameState Game::getState() {
	return state;
}

void Game::setState(GameState newState) {
	state = newState;

	switch (state) {
	case MenuMain: {
		DEBUG_TRACE("State MenuMain");
		gui.clear();

		constexpr la::Vec4 textCol = {1.0f, 1.0f, 1.0f, 1.0f};
		constexpr la::Vec4 backCol = {1.0f, 1.0f, 1.0f, 0.2f};
		constexpr la::Vec4 selCol  = {1.0f, 1.0f, 1.0f, 0.4f};
		constexpr la::Vec2 margin  = {-6, 0};

		gui.addLabel("title", 72, "Marble Maze", {320, 80}, textCol, rn::TextAlign::Center);

		gui.addButton("play",    36, "Play",    {320, 320}, textCol, backCol, selCol, 160, margin);
		gui.addButton("options", 36, "Options", {320, 360}, textCol, backCol, selCol, 160, margin);
		gui.addButton("quit",    36, "Quit",    {320, 400}, textCol, backCol, selCol, 160, margin);
	} break;
	case MenuOptions: {
		DEBUG_TRACE("State MenuOptions");
		gui.clear();
	} break;
	case MenuLevels: {
		DEBUG_TRACE("State MenuLevels");
		gui.clear();
	} break;
	case ScenePlaying: {
		DEBUG_TRACE("State ScenePlaying");
		gui.clear();
	} break;
	case ScenePaused: {
		DEBUG_TRACE("State ScenePaused");
		gui.clear();

		gui.addLabel("paused", 24, "Press P to resume", {320, 440});
	} break;
	case SceneWin: {
		DEBUG_TRACE("State SceneWin");
		gui.clear();

		std::stringstream timeText;
		timeText << "Time: " << std::setprecision(4) << scene.getTime() << "s";

		constexpr la::Vec4 textCol = {1.0f, 1.0f, 1.0f, 1.0f};
		constexpr la::Vec4 backCol = {1.0f, 1.0f, 1.0f, 0.2f};
		constexpr la::Vec4 selCol  = {1.0f, 1.0f, 1.0f, 0.4f};
		constexpr la::Vec2 margin  = {-6, 0};

		gui.addLabel("win", 48, timeText.str(), {320, 320}, textCol, rn::TextAlign::Center);

		gui.addButton("play", 36, "Play again",   {320, 360}, textCol, backCol, selCol, 200, margin);
		gui.addButton("quit", 36, "Quit to menu", {320, 400}, textCol, backCol, selCol, 200, margin);
	} break;
	default:
		DEBUG_WARNING("Invalid state %d, not changing", state);
		break;
	}
}

bool Game::onStateMenuMain(
		float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input) {

	scene.updateCamera(deltaTime / 4, 0, 0);

	la::Vec2 scaledMouse = internalPosition(input.getMouseX(), input.getMouseY());

	bool onPlay = gui.checkButton("play", scaledMouse.x, scaledMouse.y);
	bool onOptions = gui.checkButton("options", scaledMouse.x, scaledMouse.y);
	bool onQuit = gui.checkButton("quit", scaledMouse.x, scaledMouse.y);

	if (input.getMouseL() != in::JustPressed) {
		return true;
	}

	if (onQuit) {
		return false;
	}
	if (onOptions) {
		DEBUG_TRACE("GO TO OPTIONS");
	}
	if (onPlay) {
		scene.destroy();
		scene.createFromFile(_RES_PATH "testLevel.txt", resource);
		scene.setProjection(72 * la::DegToRad, frameSize.x / frameSize.y);
		setState(ScenePlaying);
		resource.getSound("select").play();
	}
	return true;
}

bool Game::onStateMenuOptions(
		float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input) {

	return true;
}

bool Game::onStateMenuLevels(
		float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input) {

	return true;
}

bool Game::onStateScenePlaying(
		float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input) {

	if (input.getKey(in::GameRestart) == in::JustPressed) {
		resource.getSound("select").play();
		scene.restart();
	}

	if (input.getKey(in::GamePause) == in::JustPressed) {
		setState(ScenePaused);
		return true;
	}

	if (scene.checkWinCondition() || input.getKey(in::MenuSelect) == in::JustPressed) {
		resource.getSound("select").play();
		setState(SceneWin);
		return true;
	}

	if (scene.shouldPlaySound()) {
		resource.getSound("solid").play();
	}

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
	scene.updateTimer(deltaTime);

	return true;
}

bool Game::onStateScenePaused(
		float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input) {

	if (input.getKey(in::GamePause) == in::JustPressed) {
		resource.getSound("select").play();
		setState(ScenePlaying);
	}

	return true;
}

bool Game::onStateSceneWin(
		float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input) {

	la::Vec2 scaledMouse = internalPosition(input.getMouseX(), input.getMouseY());

	bool onPlay = gui.checkButton("play", scaledMouse.x, scaledMouse.y);
	bool onQuit = gui.checkButton("quit", scaledMouse.x, scaledMouse.y);

	if (input.getMouseL() != in::JustPressed) {
		return true;
	}

	if (onPlay) {
		scene.restart();
		setState(ScenePlaying);
	}
	if (onQuit) {
		setState(MenuMain);
	}
	return true;
}

la::Vec2 Game::internalPosition(float x, float y) {
	float ox = (frameSize.x - internalSize.x / internalSize.y * frameSize.y) / 2;
	return {(x - ox) / frameSize.y * internalSize.y, y / frameSize.y * internalSize.y};
}
