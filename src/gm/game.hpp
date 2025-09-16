#pragma once
#include <gm/scene.hpp>
#include <in/input.hpp>
#include <ren/gui.hpp>
#include <ren/renderassetmanager.hpp>

namespace gm {

	class Game {

	public:
		Game();

		void onInit(int width, int height, ren::RenderAssetManager& ram);
		bool onUpdate(float deltaTime, float currentTime, const in::Input& input);
		void onRender(float deltaTime, float currentTime, ren::RenderAssetManager& ram);
		void onResize(int width, int height);

	private:
		ren::GUI gui;
		Scene scene;
	};
}
