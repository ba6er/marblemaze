#include <gm/game.hpp>
#include <rn/renderer.hpp>
#include <filesystem>
#include <iomanip>
#include <set>
#include <sstream>

using namespace gm;

GameOptions GameOptions::DefaultValues() {
	return {0.2f, 0.2f, 10.0f, 1.0f, 1.0f};
}

Game::Game()
	: options(GameOptions::DefaultValues()), state(MenuMain), frameSize({0, 0}), internalSize({0, 0}), gui()
	, menuCameraYaw(0), menuCameraPitch(0), menuCameraDistance(0)
	, selectedSceneIndex(0), numLoadedScenes(0), scenes(), menuScene(), currentScene(nullptr) {}

void Game::onInit(int width, int height, float internalWidth, float internalHeight, rs::ResourceManager& resource) {
	rn::Renderer::resizeFrame(width, height);
	frameSize = {(float)width, (float)height};
	internalSize = {internalWidth, internalHeight};

	gui.create(resource.getShader("text"), resource.getFont("noto48"), resource.createMesh("gui", 600));
	gui.setFrame(0, width, 0, height);

	menuScene.createMenuScene(resource);
	menuScene.setProjection(72 * la::DegToRad, frameSize.x / frameSize.y);

	std::set<std::filesystem::path> sceneFileNames;
	for (const auto& entry : std::filesystem::directory_iterator(_RES_PATH "scenes")) {
		sceneFileNames.insert(entry.path());
	}
	for (const auto& fileName : sceneFileNames) {
		scenes.push_back(Scene());
		if (scenes[numLoadedScenes].createFromFile(fileName.c_str(), resource)) {
			DEBUG_TRACE("Loaded scene \"%s\" from %s", scenes[numLoadedScenes].getId().data(), fileName.c_str());
			scenes[numLoadedScenes].setProjection(72 * la::DegToRad, frameSize.x / frameSize.y);
			numLoadedScenes++;
		} else {
			CRITICAL_ERROR("Failed to load scene from %s", fileName.c_str());
			scenes[numLoadedScenes].destroy();
		}
	}

	currentScene = &menuScene;

	menuCameraYaw = currentScene->getCameraYaw();
	menuCameraPitch = currentScene->getCameraPitch();
	menuCameraDistance = currentScene->getCameraDistance();

	setState(MenuMain);
}

void Game::onResize(int width, int height) {
	frameSize = {(float)width, (float)height};
	rn::Renderer::resizeFrame(width, height);

	float ox = (internalSize.x - width * internalSize.y / height) / 2;
	gui.setFrame(ox, internalSize.x - ox, 0, internalSize.y);

	currentScene->setProjection(72 * la::DegToRad, frameSize.x / frameSize.y);
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

	currentScene->display();
	gui.display();
}

GameState Game::getState() {
	return state;
}

void Game::setState(GameState newState) {
	state = newState;

	constexpr la::Vec4 textCol = {1.0f, 1.0f, 1.0f, 1.0f};
	constexpr la::Vec4 backCol = {1.0f, 1.0f, 1.0f, 0.2f};
	constexpr la::Vec4 selCol  = {1.0f, 1.0f, 1.0f, 0.4f};
	constexpr la::Vec2 margin  = {-6, 0};
	constexpr la::Vec2 marginL = { 0, 2};

	switch (state) {
	case MenuMain: {
		DEBUG_TRACE("State MenuMain");
		gui.clear();

		gui.addLabel("title", 72, "Marble Maze", {320, 80}, textCol, rn::TextAlign::Center);

		gui.addButton("play",    36, "Play",    {320, 320}, textCol, backCol, selCol, 160, margin);
		gui.addButton("options", 36, "Options", {320, 360}, textCol, backCol, selCol, 160, margin);
		gui.addButton("quit",    36, "Quit",    {320, 400}, textCol, backCol, selCol, 160, margin);
	} break;
	case MenuOptions: {
		DEBUG_TRACE("State MenuOptions");
		gui.clear();

		gui.addLabel("title", 36, "Options", {320, 30}, textCol, rn::TextAlign::Center);

		gui.addButton("back", 24, "Back", {580, 24}, textCol, backCol, selCol, 96, marginL);
	} break;
	case MenuLevels: {
		DEBUG_TRACE("State MenuLevels");
		gui.clear();

		gui.addLabel("title", 36, "Select a level", {320, 30}, textCol, rn::TextAlign::Center);

		gui.addButton("play", 24, "Play", {320, 442}, textCol, backCol, selCol, 96, marginL);
		gui.addButton("prev", 24, "<",    {216, 442}, textCol, backCol, selCol, 96, marginL);
		gui.addButton("next", 24, ">",    {424, 442}, textCol, backCol, selCol, 96, marginL);
		gui.addButton("back", 24, "Back", {580,  24}, textCol, backCol, selCol, 96, marginL);
	} break;
	case ScenePlaying: {
		DEBUG_TRACE("State ScenePlaying");
		gui.clear();
	} break;
	case ScenePaused: {
		DEBUG_TRACE("State ScenePaused");
		gui.clear();

		gui.addLabel("paused", 36, "Press P to resume", {320, 400});
	} break;
	case SceneWin: {
		DEBUG_TRACE("State SceneWin");
		gui.clear();

		std::stringstream timeText;
		timeText << "Time: " << std::setprecision(4) << currentScene->getTime() << "s";

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

	menuCameraYaw += deltaTime / 4;
	currentScene->setCameraValues(menuCameraYaw, menuCameraPitch, menuCameraDistance);

	la::Vec2 scaledMouse = internalPosition(input.getMouseX(), input.getMouseY());

	bool onPlay = gui.checkButtonSelected("play", scaledMouse.x, scaledMouse.y);
	gui.setButtonSelected("play", onPlay);
	bool onOptions = gui.checkButtonSelected("options", scaledMouse.x, scaledMouse.y);
	gui.setButtonSelected("options", onOptions);
	bool onQuit = gui.checkButtonSelected("quit", scaledMouse.x, scaledMouse.y);
	gui.setButtonSelected("quit", onQuit);

	if (input.getMouseL() != in::JustPressed) {
		return true;
	}

	if (onQuit) {
		return false;
	}
	if (onOptions) {
		resource.getSound("select").play();
		setState(MenuOptions);
	}
	else if (onPlay) {
		resource.getSound("select").play();
		currentScene = &scenes[selectedSceneIndex];
		currentScene->setCameraValues(menuCameraYaw, menuCameraPitch, menuCameraDistance);
		currentScene->setProjection(72 * la::DegToRad, frameSize.x / frameSize.y);
		setState(MenuLevels);
	}
	return true;
}

bool Game::onStateMenuOptions(
		float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input) {

	int pressed[] = {
		input.getKey(in::MazeYawDecrease) == in::Pressed,
		input.getKey(in::MazeYawIncrease) == in::Pressed,
		input.getKey(in::MazeRollDecrease) == in::Pressed,
		input.getKey(in::MazeRollIncrease) == in::Pressed,
	};

	if (pressed[3] || pressed[2] || pressed[1] || pressed[0]) {
		currentScene->updateMazeRotation(
			(pressed[1] - pressed[0]) * deltaTime * options.mazeYawSensitivity,
			(pressed[3] - pressed[2]) * deltaTime * options.mazeRollSensitivity);
	}

	if (input.getMouseL() == in::Pressed || input.getScrollY() != 0) {
		currentScene->updateCamera(
			input.getDeltaMouseX() * deltaTime * options.mouseSensitivityX,
			input.getDeltaMouseY() * deltaTime * options.mouseSensitivityY,
			-input.getScrollY() * deltaTime * options.scrollSensitivity);
		menuCameraYaw = currentScene->getCameraYaw();
		menuCameraPitch = currentScene->getCameraPitch();
		menuCameraDistance = currentScene->getCameraDistance();
	}

	la::Vec2 scaledMouse = internalPosition(input.getMouseX(), input.getMouseY());

	bool onBack = gui.checkButtonSelected("back", scaledMouse.x, scaledMouse.y);
	gui.setButtonSelected("back", onBack);

	if (input.getMouseL() != in::JustPressed) {
		return true;
	}

	if (onBack) {
		resource.getSound("select").play();
		currentScene->restart();
		menuCameraDistance = currentScene->getCameraDistance();
		currentScene->setCameraValues(menuCameraYaw, menuCameraPitch, menuCameraDistance);
		currentScene->setProjection(72 * la::DegToRad, frameSize.x / frameSize.y);
		setState(MenuMain);
	}

	return true;
}

bool Game::onStateMenuLevels(
		float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input) {

	menuCameraYaw += deltaTime / 4;
	currentScene->setCameraValues(menuCameraYaw, menuCameraPitch, menuCameraDistance);

	la::Vec2 scaledMouse = internalPosition(input.getMouseX(), input.getMouseY());

	bool onPlay = gui.checkButtonSelected("play", scaledMouse.x, scaledMouse.y);
	gui.setButtonSelected("play", onPlay);
	bool onNext = gui.checkButtonSelected("next", scaledMouse.x, scaledMouse.y);
	gui.setButtonSelected("next", onNext);
	bool onPrev = gui.checkButtonSelected("prev", scaledMouse.x, scaledMouse.y);
	gui.setButtonSelected("prev", onPrev);
	bool onBack = gui.checkButtonSelected("back", scaledMouse.x, scaledMouse.y);
	gui.setButtonSelected("back", onBack);

	if (input.getMouseL() != in::JustPressed) {
		return true;
	}

	if (onPlay) {
		resource.getSound("select").play();
		currentScene->restart();
		setState(ScenePlaying);
	}
	else if (onNext) {
		resource.getSound("select").play();
		selectedSceneIndex++;
		if (selectedSceneIndex >= numLoadedScenes) {
			selectedSceneIndex -= numLoadedScenes;
		}
		currentScene = &scenes[selectedSceneIndex];
		currentScene->setCameraValues(menuCameraYaw, menuCameraPitch, menuCameraDistance);
		currentScene->setProjection(72 * la::DegToRad, frameSize.x / frameSize.y);
	}
	else if (onPrev) {
		resource.getSound("select").play();
		selectedSceneIndex--;
		if (selectedSceneIndex < 0) {
			selectedSceneIndex += numLoadedScenes;
		}
		currentScene = &scenes[selectedSceneIndex];
		currentScene->setCameraValues(menuCameraYaw, menuCameraPitch, menuCameraDistance);
		currentScene->setProjection(72 * la::DegToRad, frameSize.x / frameSize.y);
	}
	else if (onBack) {
		resource.getSound("select").play();
		setState(MenuMain);
	}

	return true;
}

bool Game::onStateScenePlaying(
		float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input) {

	if (input.getKey(in::GameRestart) == in::JustPressed) {
		resource.getSound("select").play();
		currentScene->restart();
	}

	if (input.getKey(in::GamePause) == in::JustPressed) {
		setState(ScenePaused);
		return true;
	}

	if (currentScene->checkWinCondition() || input.getKey(in::MenuSelect) == in::JustPressed) {
		resource.getSound("select").play();
		setState(SceneWin);
		return true;
	}

	if (currentScene->shouldPlaySound()) {
		resource.getSound("solid").play();
	}

	int pressed[] = {
		input.getKey(in::MazeYawDecrease) == in::Pressed,
		input.getKey(in::MazeYawIncrease) == in::Pressed,
		input.getKey(in::MazeRollDecrease) == in::Pressed,
		input.getKey(in::MazeRollIncrease) == in::Pressed,
	};

	if (pressed[3] || pressed[2] || pressed[1] || pressed[0]) {
		currentScene->updateMazeRotation(
			(pressed[1] - pressed[0]) * deltaTime * options.mazeYawSensitivity,
			(pressed[3] - pressed[2]) * deltaTime * options.mazeRollSensitivity);
	}

	if (input.getMouseL() == in::Pressed || input.getScrollY() != 0) {
		currentScene->updateCamera(
			input.getDeltaMouseX() * deltaTime * options.mouseSensitivityX,
			input.getDeltaMouseY() * deltaTime * options.mouseSensitivityY,
			-input.getScrollY() * deltaTime * options.scrollSensitivity);
	}

	currentScene->updatePhysics(deltaTime);
	currentScene->updateTimer(deltaTime);

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

	bool onPlay = gui.checkButtonSelected("play", scaledMouse.x, scaledMouse.y);
	gui.setButtonSelected("play", onPlay);
	bool onQuit = gui.checkButtonSelected("quit", scaledMouse.x, scaledMouse.y);
	gui.setButtonSelected("quit", onQuit);

	if (input.getMouseL() != in::JustPressed) {
		return true;
	}

	if (onPlay) {
		currentScene->restart();
		setState(ScenePlaying);
	}
	if (onQuit) {
		menuCameraYaw = currentScene->getCameraYaw();
		menuCameraPitch = currentScene->getCameraPitch();
		menuCameraDistance = currentScene->getCameraDistance();

		setState(MenuMain);
	}
	return true;
}

la::Vec2 Game::internalPosition(float x, float y) {
	float ox = (frameSize.x - internalSize.x / internalSize.y * frameSize.y) / 2;
	return {(x - ox) / frameSize.y * internalSize.y, y / frameSize.y * internalSize.y};
}
