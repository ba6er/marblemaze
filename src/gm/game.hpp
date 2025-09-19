#pragma once
#include <gm/scene.hpp>
#include <in/input.hpp>
#include <rn/gui.hpp>
#include <rs/resourcemanager.hpp>

namespace gm {

	enum class GameState {
		MenuMain,
		MenuOptions,
		MenuLevels,
		ScenePlaying,
		ScenePaused,
	};

	class Game {

	public:
		Game();

		void onInit(int width, int height, float internalWidth, float internalHeight, rs::ResourceManager& resource);
		bool onUpdate(float deltaTime, float currentTime, rs::ResourceManager& resource, const in::Input& input);
		void onRender(float deltaTime, float currentTime, rs::ResourceManager& resource);
		void onResize(int width, int height);

		GameState getState();
		void setState(GameState state);

	private:
		void setStateMenuMain();
		void setStateMenuOption();
		void setStateMenuLevels();
		void setStateScenePlaying();
		void setStateScenePaused();

		GameState state;

		la::Vec2 frameSize, internalSize;
		rn::GUI gui;
		Scene scene;
	};
}
