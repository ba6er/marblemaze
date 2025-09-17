#pragma once
#include <la/linalg.hpp>
#include <rs/material.hpp>
#include <rs/mesh.hpp>

namespace rn {

	class Renderable {

	public:
		Renderable();

		void create(rs::Mesh& mesh, rs::Material& material);

		rs::Mesh* mesh;
		rs::Material* material;
		la::Mat4 transform;
	};

}
