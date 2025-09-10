#include <ren/renderassetmanager.hpp>
#include <fstream>
#include <sstream>

using namespace ren;

void RenderAssetManager::initFromConfig(std::string_view fileName) {
	std::ifstream configIn(fileName.data());
	std::string configLine = "", configLine2 = "";

	int stage = 0;
	while (std::getline(configIn, configLine)) {
		if (configLine.length() == 0 || configLine.at(0) == '#') {
			continue;
		}
		if (configLine == "SHADERS") {
			stage = 1;
			continue;
		}
		if (configLine == "TEXTURES") {
			uchar blankData[] = {0xFF, 0xFF, 0xFF, 0xFF};
			createTexture("blank").create(1, 1, blankData, true);
			DEBUG_TRACE("Loaded texture \"blank\" from data 0xFFFFFFFF");

			stage = 2;
			continue;
		}
		if (configLine == "MATERIALS") {
			stage = 3;
			continue;
		}

		if (stage == 1) {
			stringToShader(configLine);
		}
		if (stage == 2) {
			stringToTexture(configLine);
		}
		if (stage == 3) {
			std::getline(configIn, configLine2);
			stringToMaterial(configLine, configLine2);
		}
	}
	DEBUG_TRACE("Loaded asset configuration from %s", fileName.data());
}

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

Shader& RenderAssetManager::getShader(std::string_view name) {
	auto value = shaders.find(name);
	if (value == shaders.end()) {
		DEBUG_ASSERT("No shader by the name of \"%s\"", "%s", name.data());
	}
	return value->second;
}

Texture& RenderAssetManager::getTexture(std::string_view name) {
	auto value = textures.find(name);
	if (value == textures.end()) {
		DEBUG_ASSERT("No texture by the name of \"%s\"", "%s", name.data());
	}
	return value->second;
}

Material& RenderAssetManager::getMaterial(std::string_view name) {
	auto value = materials.find(name);
	if (value == materials.end()) {
		DEBUG_ASSERT("No material by the name of \"%s\"", "%s", name.data());
	}
	return value->second;
}

Mesh& RenderAssetManager::getMesh(std::string_view name) {
	auto value = meshes.find(name);
	if (value == meshes.end()) {
		DEBUG_ASSERT("No mesh by the name of \"%s\"", "%s", name.data());
	}
	return value->second;
}

void RenderAssetManager::stringToShader(const std::string& configLine) {
	std::istringstream shaderConfig(configLine);
	std::string name, vertSource, fragSource;
	shaderConfig >> name >> vertSource >> fragSource;

	std::string vsf = std::string(_RES_PATH) + std::string("shader/") + vertSource;
	std::string fsf = std::string(_RES_PATH) + std::string("shader/") + fragSource;

	createShader(name).create(vsf.c_str(), fsf.c_str());

	DEBUG_TRACE("Loaded shader \"%s\" from \"%s\" and \"%s\"", name.c_str(), vertSource.c_str(), fragSource.c_str());
}

void RenderAssetManager::stringToTexture(const std::string& configLine) {
	std::istringstream textureConfig(configLine);
	std::string name, fileName, filtered;
	textureConfig >> name >> fileName >> filtered;

	std::string isf = std::string(_RES_PATH) + std::string("graphics/") + fileName;

	createTexture(name).create(isf.c_str(), filtered == "T");

	DEBUG_TRACE("Loaded texture \"%s\" from \"%s\"", name.c_str(), fileName.c_str());
}

void RenderAssetManager::stringToMaterial(const std::string& configLine1, const std::string& configLine2) {
	std::istringstream materialConfig1(configLine1);
	std::string name, shader, texture;
	int numTextures = 0;
	materialConfig1 >> name >> shader >> numTextures;

	Material& m = createMaterial(name);
	m.create(getShader(shader));
	for (int i = 0; i < numTextures; i++) {
		materialConfig1 >> texture;
		m.addTexture(i, getTexture(texture));
	}

	if (configLine2 != "-") {
		std::istringstream materialConfig2(configLine2);
		lin::Vec3 ambient, diffuse, specular;
		float shininess;
		materialConfig2 >> ambient.x >> ambient.y >> ambient.z;
		materialConfig2 >> diffuse.x >> diffuse.y >> diffuse.z;
		materialConfig2 >> specular.x >> specular.y >> specular.z;
		materialConfig2 >> shininess;

		DEBUG_TRACE("%f %f %f", ambient.x, ambient.y, ambient.z);

		m.setUniform("uMaterial.ambient", ambient / 100);
		m.setUniform("uMaterial.diffuse", diffuse / 100);
		m.setUniform("uMaterial.specular", specular / 100);
		m.setUniform("uMaterial.shininess", shininess);
	}

	DEBUG_TRACE("Loaded material \"%s\" from shader \"%s\"", name.c_str(), shader.c_str());
}

