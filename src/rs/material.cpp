#include <rs/material.hpp>

using namespace rs;

Material::Material() : shader(nullptr) {
	for (int i = 0; i < MAX_TEXTURES; i++) {
		textures[i] = nullptr;
	}
}

void Material::create(Shader& shader) {
	this->shader = &shader;
}

void Material::use() const {
	if (shader == nullptr) {
		DEBUG_WARNING("No shader set for material");
		return;
	}

	for (auto &u : uniforms) {
		shader->setUniform(u.first.c_str(), u.second);
	}
	for (int i = 0; i < MAX_TEXTURES; i++) {
		if (textures[i] == nullptr) {
			continue;
		}
		char uniformName[] = "uTexture0";
		uniformName[8] += i % 10;
		shader->setUniform(uniformName, i);
		textures[i]->use(i);
	}
	shader->use();
}

void Material::addTexture(int index, Texture& texture) {
	if (index >= MAX_TEXTURES) {
		DEBUG_ERROR("Index %d is larger than maximum allowed textures %d", index, MAX_TEXTURES);
		return;
	}
	textures[index] = &texture;
}

void Material::setUniform(const std::string& name, const UniformType& value) {
	uniforms[name] = value;
}


