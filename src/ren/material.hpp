#pragma once

#pragma once
#include <lin/linalg.hpp>
#include <ren/shader.hpp>
#include <ren/texture.hpp>
#include <util.hpp>
#include <map>
#include <string>

namespace ren {

	class Material {

	public:
		Material();

		void create(Shader& shader);

		void use() const;

		void addTexture(int index, Texture& texture);
		void setUniform(const std::string& name, const UniformType& value);

	private:
		static constexpr int MAX_TEXTURES = 4;

		Shader* shader;
		Texture* textures[MAX_TEXTURES];
		std::map<std::string, UniformType> uniforms;
	};
}
