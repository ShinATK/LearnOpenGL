#version 450 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light pointLight;

uniform vec3 viewPos;

void main()
{
	// 环境光
	vec3 ambient = pointLight.ambient * vec3(texture(texture_diffuse1, TexCoord));

	vec3 norm = normalize(Normal);

	// 漫反射
	vec3 lightDir = normalize(pointLight.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = pointLight.diffuse * diff * vec3(texture(texture_diffuse1, TexCoord));

	// 镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	// vec3 specular = (pointLight[0].specular * spec_0 + pointLight[1].specular * spec_1) * vec3(texture(texture_specular1, TexCoord));
	vec3 specular = pointLight.specular * spec * vec3(texture(texture_specular1, TexCoord));

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
