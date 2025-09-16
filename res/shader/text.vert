#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexture;
layout (location = 3) in vec3 vNormal;

out vec4 iModulate;
out vec2 iTexCoord;

uniform mat4 projection;

void main() {
  gl_Position = projection * vec4(vPosition, 1.0);
  iModulate = vec4(vColor, 1.0);
  iTexCoord = vTexture;
}
