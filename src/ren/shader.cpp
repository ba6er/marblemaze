#include <ren/shader.hpp>
#include <glad/glad.h>
#include <fstream>

using namespace ren;

Shader::Shader() : id(0) {}

void Shader::create(cstr vertex, cstr fragment) {
	// Vertex shader
	std::ifstream vertIn(vertex);
	std::string vertString((std::istreambuf_iterator<char>(vertIn)), std::istreambuf_iterator<char>());
	cstr vertSource = vertString.c_str();

	uint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertSource, NULL);
	glCompileShader(vertShader);
	CheckCompileErrors(vertShader);

	// Fragment shader
	std::ifstream fragIn(fragment);
	std::string fragString((std::istreambuf_iterator<char>(fragIn)), std::istreambuf_iterator<char>());
	cstr frag_source = fragString.c_str();

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

void Shader::setUniform(cstr name, const UniformType& value) const {
	glUseProgram(id);
	if (std::holds_alternative<int>(value)) {
		glUniform1i(glGetUniformLocation(id, name), std::get<int>(value));
	}
	else if (std::holds_alternative<uint>(value)) {
		glUniform1ui(glGetUniformLocation(id, name), std::get<uint>(value));
	}
	else if (std::holds_alternative<float>(value)) {
		glUniform1f(glGetUniformLocation(id, name), std::get<float>(value));
	}
	else if (std::holds_alternative<lin::Mat4>(value)) {
		glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_TRUE, &std::get<lin::Mat4>(value).x1);
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
