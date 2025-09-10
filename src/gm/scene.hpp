#pragma once
#include <gm/maze.hpp>
#include <ren/camera.hpp>
#include <ren/renderable.hpp>
#include <ren/renderer.hpp>
#include <vector>

namespace gm {

	class Scene {

	public:
		void updatePhysics(float deltaTime);

		ren::Camera camera;
		ren::Light light;
		std::vector<ren::Renderable> renderables;

		Maze maze;
	};
}
