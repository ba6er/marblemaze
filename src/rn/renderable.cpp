#include <rn/renderable.hpp>

using namespace rn;
using namespace rs;

Renderable::Renderable() : mesh(nullptr), material(nullptr), transform(la::Mat4::Identity()) {}

void Renderable::create(Mesh& mesh, Material& material) {
	this->mesh = &mesh;
	this->material = &material;
}
