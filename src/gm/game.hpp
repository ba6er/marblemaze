#pragma once
#include <in/input.hpp>
#include <ren/renderassetmanager.hpp>

namespace gm {

	class Game {

	public:
		void onInit(ren::RenderAssetManager& ram);
		bool onUpdate(float deltaTime, float currentTime, const in::Input& input);
		void onRender(float deltaTime, float currentTime, ren::RenderAssetManager& ram);
	};
}
