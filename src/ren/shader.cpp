#include <ren/shader.hpp>
#include <glad/glad.h>
#include <fstream>

using namespace ren;

Shader::Shader() : id(0) {}

void Shader::create(std::string_view vertex, std::string_view fragment) {
	// Vertex shader
	std::ifstream vertIn(vertex.data());
	std::string vertString((std::istreambuf_iterator<char>(vertIn)), std::istreambuf_iterator<char>());
	const char* vertSource = vertString.c_str();

	uint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertSource, NULL);
	glCompileShader(vertShader);
	CheckCompileErrors(vertShader);

	// Fragment shader
	std::ifstream fragIn(fragment.data());
	std::string fragString((std::istreambuf_iterator<char>(fragIn)), std::istreambuf_iterator<char>());
	const char* frag_source = fragString.c_str();

	uint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &frag_source, NULL);
	glCompileShader(fragShader);
	CheckCompileErrors(fragShader);

	// Shader program
	id = glCreateProgram();
	glAttachShader(id, vertShader);
	glAttachShader(id, fragShader);
	glLinkProgram(id);
	CheckLinkingErrors(id);

	// Cleanup
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

void Shader::destroy() {
	glDeleteProgram(id);
}

void Shader::use() const {
	glUseProgram(id);
}

void Shader::setUniform(std::string_view name, const UniformType& value) const {
	glUseProgram(id);
	if (std::holds_alternative<int>(value)) {
		glUniform1i(glGetUniformLocation(id, name.data()), std::get<int>(value));
	}
	else if (std::holds_alternative<uint>(value)) {
		glUniform1ui(glGetUniformLocation(id, name.data()), std::get<uint>(value));
	}
	else if (std::holds_alternative<float>(value)) {
		glUniform1f(glGetUniformLocation(id, name.data()), std::get<float>(value));
	}
	else if (std::holds_alternative<lin::Vec2>(value)) {
		lin::Vec2 v = std::get<lin::Vec2>(value);
		glUniform2f(glGetUniformLocation(id, name.data()), v.x, v.y);
	}
	else if (std::holds_alternative<lin::Vec3>(value)) {
		lin::Vec3 v = std::get<lin::Vec3>(value);
		glUniform3f(glGetUniformLocation(id, name.data()), v.x, v.y, v.z);
	}
	else if (std::holds_alternative<lin::Vec4>(value)) {
		lin::Vec4 v = std::get<lin::Vec4>(value);
		glUniform4f(glGetUniformLocation(id, name.data()), v.x, v.y, v.z, v.w);
	}
	else if (std::holds_alternative<lin::Mat4>(value)) {
		glUniformMatrix4fv(glGetUniformLocation(id, name.data()), 1, GL_TRUE, &std::get<lin::Mat4>(value).x1);
	}
	glUseProgram(0);
}

void Shader::CheckCompileErrors(uint shader) {
	int success;
	char infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success != 0) {
		return;
	}

	glGetShaderInfoLog(shader, 512, NULL, infoLog);
	DEBUG_ASSERT(0, "%s", infoLog);
}

void Shader::CheckLinkingErrors(uint program) {
	int success;
	char infoLog[512];

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success != 0) {
		return;
	}

	glGetProgramInfoLog(program, 512, NULL, infoLog);
	DEBUG_ASSERT(0, "%s", infoLog);
}
