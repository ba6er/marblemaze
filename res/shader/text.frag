#version 330 core

out vec4 fragColor;

in vec4 iModulate;
in vec2 iTexCoord;

uniform sampler2D uTexture0;

void main() {
  vec4 alpha = texture(uTexture0, iTexCoord);
  fragColor = vec4(iModulate.rgb, alpha.r);
}
