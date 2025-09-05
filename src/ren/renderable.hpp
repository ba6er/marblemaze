#pragma once
#include <lin/linalg.hpp>
#include <ren/material.hpp>
#include <ren/mesh.hpp>

namespace ren {

	class Renderable {

	public:
		Renderable();

		void create(Mesh& mesh, Material& material);

		Mesh* mesh;
		Material* material;
		lin::Mat4 transform;
	};

}
