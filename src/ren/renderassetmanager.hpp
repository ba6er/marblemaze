#pragma once
#include <ren/shader.hpp>
#include <ren/texture.hpp>
#include <ren/material.hpp>
#include <ren/mesh.hpp>
#include <util.hpp>
#include <map>
#include <string>
#include <string_view>

namespace ren {

	class RenderAssetManager {

	public:
		void initFromConfig(const std::string& fileName);

		Shader& createShader(const std::string& name);
		Texture& createTexture(const std::string& name);
		Material& createMaterial(const std::string& name);
		Mesh& createMesh(const std::string& name);

		void destroy();

		Shader& getShader(std::string_view name);
		Texture& getTexture(std::string_view name);
		Material& getMaterial(std::string_view name);
		Mesh& getMesh(std::string_view name);

	private:
		std::map<std::string, Shader, std::less<>> shaders;
		std::map<std::string, Texture, std::less<>> textures;
		std::map<std::string, Material, std::less<>> materials;
		std::map<std::string, Mesh, std::less<>> meshes;
	};
}
