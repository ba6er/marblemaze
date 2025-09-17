#pragma once
#include <rs/texture.hpp>
#include <rs/shader.hpp>
#include <rs/font.hpp>
#include <rs/material.hpp>
#include <rs/mesh.hpp>
#include <rs/sound.hpp>
#include <util.hpp>
#include <map>
#include <string>
#include <string_view>

namespace rs {

	class ResourceManager {

	public:
		void initFromConfig(std::string_view fileName, ma_engine* audioEngine);

		Shader& createShader(const std::string& name, std::string_view vertex, std::string_view fragment);
		Texture& createTexture(const std::string& name);
		Texture& createTexture(const std::string& name, int width, int height, void* data, TextureFormat format);
		Texture& createTexture(const std::string& name, std::string_view fileName, bool filtered, int mipmaps = 4);
		Material& createMaterial(const std::string& name, std::string_view shaderName);
		Mesh& createMesh(const std::string& name, int initVerts = 6);
		Font& createFont(
			const std::string& name, const std::string& textureName, std::string_view fileName, int size, bool filtered);
		Sound& createSound(const std::string& name, std::string_view fileName);

		void destroy();

		Shader& getShader(std::string_view name);
		Texture& getTexture(std::string_view name);
		Material& getMaterial(std::string_view name);
		Mesh& getMesh(std::string_view name);
		Font& getFont(std::string_view name);
		Sound& getSound(std::string_view name);

	private:
		void stringToShader(const std::string& configLine);
		void stringToTexture(const std::string& configLine);
		void stringToMaterial(const std::string& configLine1, const std::string& configLine2);
		void stringToFont(const std::string& configLine);
		void stringToSound(const std::string& configLine);

		std::map<std::string, Shader, std::less<>> shaders;
		std::map<std::string, Texture, std::less<>> textures;
		std::map<std::string, Material, std::less<>> materials;
		std::map<std::string, Mesh, std::less<>> meshes;
		std::map<std::string, Font, std::less<>> fonts;
		std::map<std::string, Sound, std::less<>> sounds;

		ma_engine* audioEngine = nullptr;
	};
}
