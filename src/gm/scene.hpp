#pragma once
#include <ren/camera.hpp>
#include <ren/renderable.hpp>
#include <vector>

namespace gm {

	class Scene {

	public:
		void updatePhysics(float deltaTime);

		ren::Camera camera;
		std::vector<ren::Renderable> renderables;
	};
}
