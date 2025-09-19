#include <rn/renderable.hpp>

using namespace rn;
using namespace rs;

Renderable::Renderable() : tag(""), mesh(nullptr), material(nullptr), transform(la::Mat4::Identity()) {}

void Renderable::create(Mesh& mesh, Material& material, const std::string tag) {
	this->tag = tag;
	this->mesh = &mesh;
	this->material = &material;
}
