#pragma once
#include <lin/linalg.hpp>
#include <util.hpp>
#include <variant>

namespace ren {

	typedef std::variant<int, uint, float, lin::Mat4> UniformType;

	class Shader {

	public:
		bool create(cstr vertex, cstr fragment);
		void destroy();

		void use() const;

		void setUniform(cstr name, const UniformType& value) const;

	private:
		static bool CheckCompileErrors(uint shader);
		static bool CheckLinkingErrors(uint program);

		uint id;
	};
}
