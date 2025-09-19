#version 330 core

out vec4 fragColor;

in vec4 iModulate;
in vec2 iTexCoord;
in float iSolid;
in float iAlpha;

uniform sampler2D uTexture0;

void main() {
	vec4 textAlpha = texture(uTexture0, iTexCoord);
	if (iSolid < 0.5) {
		fragColor = vec4(iModulate.rgb, textAlpha.r * iAlpha);
	} else {
		fragColor = vec4(iModulate.rgb, iAlpha);
	}
}
