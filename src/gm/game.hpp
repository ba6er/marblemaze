#pragma once
#include <gm/scene.hpp>
#include <in/input.hpp>
#include <rn/gui.hpp>
#include <rs/resourcemanager.hpp>

namespace gm {

	class Game {

	public:
		Game();

		void onInit(int width, int height, rs::ResourceManager& resource);
		bool onUpdate(float deltaTime, float currentTime, const in::Input& input);
		void onRender(float deltaTime, float currentTime, rs::ResourceManager& resource);
		void onResize(int width, int height);

	private:
		rn::GUI gui;
		Scene scene;
	};
}
