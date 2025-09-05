#pragma once
#include <ren/camera.hpp>
#include <ren/renderable.hpp>
#include <vector>

namespace ren {

	class Renderer {

	public:
		static void render(Renderable& renderable);
		static void render(Camera& camera, const std::vector<Renderable>& renderables);
	};
}
