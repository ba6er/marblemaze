#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec4 vCol;
layout (location = 2) in vec2 vTex;

out vec4 iModulate;
out vec2 iTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(vPos, 1.0);
  iModulate = vCol;
  iTexCoord = vTex;
}
