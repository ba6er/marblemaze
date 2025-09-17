#pragma once
#include <rn/camera.hpp>
#include <rn/renderable.hpp>
#include <vector>

namespace rn {

	class Light {

	public:
		la::Vec3 position;

		la::Vec3 ambient;
		la::Vec3 diffuse;
		la::Vec3 specular;
	};

	class Renderer {

	public:
		static void clear(float red, float green, float blue);
		static void render(Renderable& renderable);
		static void render(const Camera& camera, const std::vector<Renderable>& renderables, const Light& light);

		static void resizeFrame(int width, int height);
	};
}
