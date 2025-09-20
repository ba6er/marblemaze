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
	: optionWholeValues({5, 5, 5, 5, 5}), options(GameOptions::DefaultValues()), state(MenuMain)
	, frameSize({0, 0}), internalSize({0, 0}), gui()
	, menuCameraYaw(0), menuCameraPitch(0), menuCameraDistance(0)
	, selectedSceneIndex(0), numLoadedScenes(0), scenes(), menuScene(), currentScene(nullptr) {}

void Game::onInit(int width, int height, float internalWidth, float internalHeight, rs::ResourceManager& resource) {
	rn::Renderer::resizeFrame(width, height);
	frameSize = {(float)width, (float)height};
	internalSize = {internalWidth, internalHeight};

	gui.create(resource.getShader("text"), resource.getFont("noto48"), resource.createMesh("gui", 3072));
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
	constexpr la::Vec2 margin  = {0, 2};

	switch (state) {
	case MenuMain: {
		DEBUG_TRACE("State MenuMain");
		gui.clear();

		gui.addLabel("title", 72, "Marble Maze", {320, 80}, textCol, rn::TextAlign::Center);

		gui.addButton("play",    24, "Play",    {320, 382}, textCol, backCol, selCol, 160, margin);
		gui.addButton("options", 24, "Options", {320, 412}, textCol, backCol, selCol, 160, margin);
		gui.addButton("quit",    24, "Quit",    {320, 442}, textCol, backCol, selCol, 160, margin);
	} break;
	case MenuOptions: {
		DEBUG_TRACE("State MenuOptions");
		gui.clear();

		gui.addLabel("title", 36, "Options", {320, 30}, textCol, rn::TextAlign::Center);
		gui.addButton("back", 24, "Back", {532, 24}, textCol, backCol, selCol, 96, margin);

		constexpr float horMargin  = 60;
		constexpr float textMargin = 20;
		constexpr float btnWidth   = 30;
		constexpr float dfltWidth  = 100;
		constexpr float valueWidth = 60;
		constexpr float textWidth  = 640 - horMargin * 2 - btnWidth * 2 - valueWidth - dfltWidth;

		constexpr float lX = horMargin + textMargin;
		gui.addLabel("mx", 24, "Horizontal mouse sensitivity", {lX, 322}, textCol, rn::TextAlign::Left);
		gui.addLabel("my", 24, "Vertical mouse sensitivity",   {lX, 352}, textCol, rn::TextAlign::Left);
		gui.addLabel("ms", 24, "Scroll-wheel sensitivity",     {lX, 382}, textCol, rn::TextAlign::Left);
		gui.addLabel("rx", 24, "Maze horizontal sensitivity",  {lX, 412}, textCol, rn::TextAlign::Left);
		gui.addLabel("ry", 24, "Maze Vertical sensitivity",    {lX, 442}, textCol, rn::TextAlign::Left);

		// Label background hack XD
		constexpr float bgX = horMargin + textWidth / 2;
		gui.addButton("mxBg", 24, "", {bgX, 322}, textCol, backCol, selCol, textWidth, margin);
		gui.addButton("myBg", 24, "", {bgX, 352}, textCol, backCol, selCol, textWidth, margin);
		gui.addButton("msBg", 24, "", {bgX, 382}, textCol, backCol, selCol, textWidth, margin);
		gui.addButton("rxBg", 24, "", {bgX, 412}, textCol, backCol, selCol, textWidth, margin);
		gui.addButton("ryBg", 24, "", {bgX, 442}, textCol, backCol, selCol, textWidth, margin);

		constexpr float vlX = bgX + textWidth / 2 + valueWidth / 2;
		gui.addButton("mxVl", 24, "5", {vlX, 322}, textCol, backCol, selCol, valueWidth, margin);
		gui.addButton("myVl", 24, "5", {vlX, 352}, textCol, backCol, selCol, valueWidth, margin);
		gui.addButton("msVl", 24, "5", {vlX, 382}, textCol, backCol, selCol, valueWidth, margin);
		gui.addButton("rxVl", 24, "5", {vlX, 412}, textCol, backCol, selCol, valueWidth, margin);
		gui.addButton("ryVl", 24, "5", {vlX, 442}, textCol, backCol, selCol, valueWidth, margin);

		constexpr float decX = vlX + valueWidth / 2 + btnWidth / 2;
		gui.addButton("0", 24, "-", {decX, 322}, textCol, backCol, selCol, btnWidth, margin);
		gui.addButton("1", 24, "-", {decX, 352}, textCol, backCol, selCol, btnWidth, margin);
		gui.addButton("2", 24, "-", {decX, 382}, textCol, backCol, selCol, btnWidth, margin);
		gui.addButton("3", 24, "-", {decX, 412}, textCol, backCol, selCol, btnWidth, margin);
		gui.addButton("4", 24, "-", {decX, 442}, textCol, backCol, selCol, btnWidth, margin);

		constexpr float incX = decX + btnWidth;
		gui.addButton("5", 24, "+", {incX, 322}, textCol, backCol, selCol, btnWidth, margin);
		gui.addButton("6", 24, "+", {incX, 352}, textCol, backCol, selCol, btnWidth, margin);
		gui.addButton("7", 24, "+", {incX, 382}, textCol, backCol, selCol, btnWidth, margin);
		gui.addButton("8", 24, "+", {incX, 412}, textCol, backCol, selCol, btnWidth, margin);
		gui.addButton("9", 24, "+", {incX, 442}, textCol, backCol, selCol, btnWidth, margin);

		constexpr float dfltX = incX + btnWidth / 2 + dfltWidth / 2;
		gui.addButton("10", 24, "Default", {dfltX, 322}, textCol, backCol, selCol, dfltWidth, margin);
		gui.addButton("11", 24, "Default", {dfltX, 352}, textCol, backCol, selCol, dfltWidth, margin);
		gui.addButton("12", 24, "Default", {dfltX, 382}, textCol, backCol, selCol, dfltWidth, margin);
		gui.addButton("13", 24, "Default", {dfltX, 412}, textCol, backCol, selCol, dfltWidth, margin);
		gui.addButton("14", 24, "Default", {dfltX, 442}, textCol, backCol, selCol, dfltWidth, margin);

	} break;
	case MenuLevels: {
		DEBUG_TRACE("State MenuLevels");
		gui.clear();

		gui.addLabel("title", 36, "Select a level", {320, 30}, textCol, rn::TextAlign::Center);
		gui.addButton("back", 24, "Back", {532,  24}, textCol, backCol, selCol, 96, margin);

		gui.addButton("play", 24, "Play", {320, 442}, textCol, backCol, selCol, 96, margin);
		gui.addButton("prev", 24, "<",    {216, 442}, textCol, backCol, selCol, 96, margin);
		gui.addButton("next", 24, ">",    {424, 442}, textCol, backCol, selCol, 96, margin);
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

		gui.addButton("play", 24, "Play again",   {320, 360}, textCol, backCol, selCol, 200, margin);
		gui.addButton("quit", 24, "Quit to menu", {320, 390}, textCol, backCol, selCol, 200, margin);
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

	bool optsBtnValues[15];
	for (int i = 0; i < 15; i++) {
		optsBtnValues[i] = gui.checkButtonSelected(std::to_string(i), scaledMouse.x, scaledMouse.y);
		gui.setButtonSelected(std::to_string(i), optsBtnValues[i]);
	}

	if (input.getMouseL() != in::JustPressed) {
		return true;
	}

	if (onBack) {
		resource.getSound("select").play();
		setState(MenuMain);
	}

	int optionChangeIndex = -1;
	for (int i = 0; i < 5; i++) {
		if (optsBtnValues[i]) {
			optionWholeValues[i % 5] = std::max(optionWholeValues[i % 5] - 1, 1);
			optionChangeIndex = i % 5;
		}
	}
	for (int i = 5; i < 10; i++) {
		if (optsBtnValues[i]) {
			optionWholeValues[i % 5] = std::min(optionWholeValues[i % 5] + 1, 10);
			optionChangeIndex = i % 5;
		}
	}
	for (int i = 10; i < 15; i++) {
		if (optsBtnValues[i]) {
			optionWholeValues[i % 5] = 5;
			optionChangeIndex = i % 5;
		}
	}

	if (optionChangeIndex == -1) {
		return true;
	}

	GameOptions dfVl = GameOptions::DefaultValues();
	int mul = optionWholeValues[optionChangeIndex];
	if (optionChangeIndex == 0) {
		gui.setLabelText("mxVl", std::to_string(mul));
		options.mouseSensitivityX = dfVl.mouseSensitivityX * mul / 5;
	}
	else if (optionChangeIndex == 1) {
		gui.setLabelText("myVl", std::to_string(mul));
		options.mouseSensitivityY = dfVl.mouseSensitivityY * mul / 5;
	}
	else if (optionChangeIndex == 2) {
		gui.setLabelText("msVl", std::to_string(mul));
		options.scrollSensitivity = dfVl.scrollSensitivity * mul / 5;
	}
	else if (optionChangeIndex == 3) {
		gui.setLabelText("rxVl", std::to_string(mul));
		options.mazeRollSensitivity = dfVl.mazeRollSensitivity * mul / 5;
	}
	else if (optionChangeIndex == 4) {
		gui.setLabelText("ryVl", std::to_string(mul));
		options.mazeYawSensitivity = dfVl.mazeYawSensitivity * mul / 5;
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
