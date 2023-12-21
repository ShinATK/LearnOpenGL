#version 450 core

// 定义物体的材质结构体
struct Material{
	sampler2D emission;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

// 定义光源的不同分量的影响
struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

// 最终的颜色结果
out vec4 FragColor;

// 计算光照的：环境光、漫反射、高光项
in vec3 Normal; // 法线
in vec3 FragPos; // 片段位置
in vec3 LightPos; // 光源位置

in vec2 TexCoord;

uniform vec3 viewPos; // 观察者位置
uniform float dynamic_texcoord;

void main()
{

	// 计算漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos); // 漫反射需要的光线方向为片段指向光源位置
	float diff = max(dot(norm, lightDir), 0.0);

	// 计算镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // 镜面反射需要的光线方向为光源指向片段位置
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// 环境光+漫反射+高光
	vec3 emission = vec3(texture(material.emission, vec2(TexCoord.x, TexCoord.y + dynamic_texcoord)));
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	// 计算最终颜色
	FragColor = vec4(specular + diffuse + ambient + emission, 1.0);
}