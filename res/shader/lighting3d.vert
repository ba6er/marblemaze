#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexture;
layout (location = 3) in vec3 vNormal;

out vec4 iModulate;
out vec2 iTextureCoord;
out vec3 iFragPosition;
out vec3 iNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(vPosition, 1.0);

	iModulate = vec4(vColor, 1.0);
	iTextureCoord = vTexture;

	iFragPosition = vec3(model * vec4(vPosition, 1.0));
	iNormal = mat3(transpose(inverse(model))) * vNormal;
}
