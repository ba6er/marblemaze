#include <ren/renderassetmanager.hpp>

using namespace ren;

Shader& RenderAssetManager::createShader(const std::string& name) {
	shaders[name] = Shader();
	return shaders.at(name);
}

Texture& RenderAssetManager::createTexture(const std::string& name) {
	textures[name] = Texture();
	return textures.at(name);
}

Material& RenderAssetManager::createMaterial(const std::string& name) {
	materials[name] = Material();
	return materials.at(name);
}

Mesh& RenderAssetManager::createMesh(const std::string& name) {
	meshes[name] = Mesh();
	return meshes.at(name);
}

void RenderAssetManager::destroy() {
	for (auto& shader : shaders) {
		shader.second.destroy();
	}
	for (auto& texture : textures) {
		texture.second.destroy();
	}
	for (auto& mesh : meshes) {
		mesh.second.destroy();
	}
}

Shader& RenderAssetManager::getShader(const std::string& name) {
	return shaders.at(name);
}

Texture& RenderAssetManager::getTexture(const std::string& name) {
	return textures.at(name);
}

Material& RenderAssetManager::getMaterial(const std::string& name) {
	return materials.at(name);
}

Mesh& RenderAssetManager::getMesh(const std::string& name) {
	return meshes.at(name);
}

