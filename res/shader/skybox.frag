#version 330 core

out vec4 fragColor;

in vec4 iModulate;
in vec2 iTexCoord;
in vec3 iNormal;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;
uniform sampler2D uTexture4;
uniform sampler2D uTexture5;

void main() {
	vec4 texCol = vec4(1.0, 1.0, 1.0, 1.0);
	if (iNormal.z < -0.5) {
		texCol = texture(uTexture0, iTexCoord);
	}
	else if (iNormal.z > 0.5) {
		texCol = texture(uTexture1, iTexCoord);
	}
	else if (iNormal.x < -0.5) {
		texCol = texture(uTexture2, iTexCoord);
	}
	else if (iNormal.x > 0.5) {
		texCol = texture(uTexture3, iTexCoord);
	}
	else if (iNormal.y < -0.5) {
		texCol = texture(uTexture4, iTexCoord);
	}
	else if (iNormal.y > 0.5) {
		texCol = texture(uTexture5, iTexCoord);
	}
	fragColor = texCol * iModulate;
}
