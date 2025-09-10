#pragma once
#include <lin/linalg.hpp>
#include <util.hpp>
#include <string_view>
#include <variant>

namespace ren {

	using UniformType = std::variant<int, uint, float, lin::Vec2, lin::Vec3, lin::Vec4, lin::Mat4>;

	class Shader {

	public:
		Shader();

		void create(std::string_view vertex, std::string_view fragment);
		void destroy();

		void use() const;

		void setUniform(std::string_view name, const UniformType& value) const;

	private:
		static void CheckCompileErrors(uint shader);
		static void CheckLinkingErrors(uint program);

		uint id;
	};
}
