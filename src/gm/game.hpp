#pragma once
#include <gm/scene.hpp>
#include <in/input.hpp>
#include <rn/gui.hpp>
#include <rs/resourcemanager.hpp>
#include <array>

namespace gm {

	enum GameState : uint {
		MenuMain     = 1 << 0,
		MenuOptions  = 1 << 1,
		MenuLevels   = 1 << 2,
		ScenePlaying = 1 << 3,
		ScenePaused  = 1 << 4,
		SceneWin     = 1 << 5,
		InMenu       = MenuMain | MenuOptions | MenuLevels,
		InScene      = ScenePlaying | ScenePaused | SceneWin,
	};

	class GameOptions {

	public:
		static constexpr float WholeMin = 1;
		static constexpr float WholeMax = 10;
		static constexpr float WholeDefault = 5;
		static constexpr size_t NumWholeValues = 5;

		GameOptions();

		int rememberValues;
		int isFullscreen;
		float mouseSensitivityX;
		float mouseSensitivityY;
		float scrollSensitivity;
		float mazeYawSensitivity;
		float mazeRollSensitivity;
		std::array<int, NumWholeValues> wholeValues;
	};

	class Game {

	public:
		Game();

		void onInit(la::Vec2 frameSize, la::Vec2 internalSize, rs::ResourceManager& resource, const GameOptions& options);
		bool onUpdate(float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input);
		void onRender(float deltaTime, float currentTime, rs::ResourceManager& resource);
		void onResize(int width, int height);

		GameState getState();
		void setState(GameState newState);

		const std::vector<Scene>& getScenes() const;

		GameOptions options;

	private:
		bool onStateMenuMain(
			float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input);
		bool onStateMenuOptions(
			float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input);
		bool onStateMenuLevels(
			float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input);
		bool onStateScenePlaying(
			float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input);
		bool onStateScenePaused(
			float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input);
		bool onStateSceneWin(
			float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input);

		la::Vec2 internalPosition(float x, float y);

		GameState state;

		la::Vec2 frameSize, internalSize;
		rn::GUI gui;

		float menuCameraYaw, menuCameraPitch, menuCameraDistance;

		int selectedSceneIndex, numLoadedScenes;
		std::vector<Scene> scenes;
		Scene menuScene;
		Scene* currentScene;
	};
}
