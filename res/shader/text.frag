#version 330 core

out vec4 fragColor;

in vec4 iModulate;
in vec2 iTexCoord;
in float iSolid;

uniform sampler2D uTexture0, uTexture1;

void main() {
	vec4 alpha = texture(uTexture0, iTexCoord);
	fragColor = vec4(iModulate.rgb, alpha.r);
}
