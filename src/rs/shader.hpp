#pragma once
#include <la/linalg.hpp>
#include <util.hpp>
#include <string_view>
#include <variant>

namespace rs {

	using UniformType = std::variant<int, uint, float, la::Vec2, la::Vec3, la::Vec4, la::Mat4>;

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
