#include <ren/renderassetmanager.hpp>
#include <fstream>
#include <sstream>

using namespace ren;

void RenderAssetManager::initFromConfig(cstr fileName) {
	std::ifstream configIn(fileName);
	std::string configLine = "";

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

		// Shaders
		if (stage == 1) {
			std::istringstream shaderConfig(configLine);
			std::string name, vertSource, fragSource;
			shaderConfig >> name >> vertSource >> fragSource;
			std::string vsf = std::string(_RES_PATH) + std::string("shader/") + vertSource;
			std::string fsf = std::string(_RES_PATH) + std::string("shader/") + fragSource;
			createShader(name).create(vsf.c_str(), fsf.c_str());
			DEBUG_TRACE("Loaded shader \"%s\" from \"%s\" and \"%s\"", name.c_str(), vertSource.c_str(), fragSource.c_str());
		}

		// Textures
		if (stage == 2) {
			std::istringstream textureConfig(configLine);
			std::string name, fileName, filtered;
			textureConfig >> name >> fileName >> filtered;
			std::string isf = std::string(_RES_PATH) + std::string("graphics/") + fileName;
			createTexture(name).create(isf.c_str(), filtered == "T");
			DEBUG_TRACE("Loaded texture \"%s\" from \"%s\"", name.c_str(), fileName.c_str());
		}

		// Materials
		if (stage == 3) {
			std::istringstream materialConfig(configLine);
			std::string name, shader, texture;
			int numTextures = 0;
			materialConfig >> name >> shader >> numTextures;
			Material& m = createMaterial(name);
			m.create(getShader(shader));
			for (int i = 0; i < numTextures; i++) {
				materialConfig >> texture;
				m.addTexture(i, getTexture(name));
			}
			DEBUG_TRACE("Loaded material \"%s\" from shader \"%s\"", name.c_str(), shader.c_str());
		}
	}
	DEBUG_TRACE("Loaded asset configuration from %s", fileName);
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

