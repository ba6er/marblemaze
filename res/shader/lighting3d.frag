#version 330 core

out vec4 fragColor;

in vec4 iModulate;
in vec2 iTextureCoord;
in vec3 iFragPosition;
in vec3 iNormal;

uniform vec3 uViewPosition;

uniform sampler2D uTexture0;

vec3 lightPosition = vec3(0.0, 0.0, 2.0);
vec3 ambientLight = vec3(0.2, 0.2, 0.2);

float specularStrength = 0.5;
int shininess = 16;

void main() {
  vec4 baseColor = texture(uTexture0, iTextureCoord) * iModulate;

  vec3 ambient = ambientLight;

  vec3 normal = normalize(iNormal);

  vec3 lightDir = normalize(lightPosition - iFragPosition);
  vec3 diffuse = max(dot(normal, lightDir) / 2 + 0.5, 0.0) * vec3(1, 1, 1);

  vec3 viewDir = normalize(uViewPosition - iFragPosition);
  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), shininess) * specularStrength * vec3(1, 1, 1);

  fragColor = vec4(ambient + diffuse + specular, 1.0) * baseColor;
}
