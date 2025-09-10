#pragma once
#include <ren/camera.hpp>
#include <ren/renderable.hpp>
#include <vector>

namespace ren {

	class Light {

	public:
		lin::Vec3 position;

		lin::Vec3 ambient;
		lin::Vec3 diffuse;
		lin::Vec3 specular;
	};

	class Renderer {

	public:
		static void clear(float red, float green, float blue);
		static void render(Renderable& renderable);
		static void render(const Camera& camera, const std::vector<Renderable>& renderables, const Light& light);

		static void resizeFrame(int width, int height);
	};
}
