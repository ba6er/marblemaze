#version 330 core

out vec4 fragColor;

in vec4 iModulate;

void main() {
  fragColor = iModulate;
}
