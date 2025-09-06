#pragma once
#include <in/input.hpp>
#include <ren/renderassetmanager.hpp>

namespace gm {

	class Game {

	public:
		bool onUpdate(float deltaTime, float currentTime, const in::Input& input);
		void onRender(float deltaTime, float currentTime, ren::RenderAssetManager& ram);
	};
}
