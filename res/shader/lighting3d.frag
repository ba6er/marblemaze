#version 330 core

out vec4 fragColor;

in vec4 iModulate;
in vec2 iTextureCoord;
in vec3 iFragPosition;
in vec3 iNormal;

uniform vec3 uViewPosition;

struct Material {
	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
	float shininess;
};
uniform Material uMaterial;

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light uLight;

uniform sampler2D uTexture0;

void main() {
	vec3 normal = normalize(iNormal);
	vec4 baseColor = texture(uTexture0, iTextureCoord) * iModulate;

	vec3 lightDir = normalize(uLight.position - iFragPosition);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 viewDir = normalize(uViewPosition - iFragPosition);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);

	vec3 ambient  = uLight.ambient * uMaterial.ambient;
	vec3 diffuse  = uLight.diffuse * (diff * uMaterial.diffuse);
	vec3 specular = uLight.specular * (spec * uMaterial.specular);

	fragColor = vec4(ambient + diffuse + specular, 1.0) * baseColor;
}
