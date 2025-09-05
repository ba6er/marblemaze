#pragma once
#include <ren/shader.hpp>
#include <ren/texture.hpp>
#include <ren/material.hpp>
#include <ren/mesh.hpp>
#include <map>
#include <string>

namespace ren {

	class RenderAssetManager {

	public:
		Shader& createShader(const std::string& name);
		Texture& createTexture(const std::string& name);
		Material& createMaterial(const std::string& name);
		Mesh& createMesh(const std::string& name);

		void destroy();

		Shader& getShader(const std::string& name);
		Texture& getTexture(const std::string& name);
		Material& getMaterial(const std::string& name);
		Mesh& getMesh(const std::string& name);

	private:
		std::map<std::string, Shader> shaders;
		std::map<std::string, Texture> textures;
		std::map<std::string, Material> materials;
		std::map<std::string, Mesh> meshes;
	};
}
