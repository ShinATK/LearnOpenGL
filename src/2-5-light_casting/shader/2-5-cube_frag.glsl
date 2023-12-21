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
	vec3 direction;
	float CutOff;		//	内切光角
	float outerCutOff;	// 外切光角

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
uniform Light light;

// 最终的颜色结果
out vec4 FragColor;

// 计算光照的：环境光、漫反射、高光项
in vec3 Normal; // 法线
in vec3 FragPos; // 片段位置

in vec2 TexCoord;

uniform vec3 viewPos; // 观察者位置

void main()
{
	// 法线
	vec3 norm = normalize(Normal);

	// 计算光源方向
	vec3 lightDir = normalize(light.position - FragPos);

	// 模拟手电筒聚光
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.CutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0);

	// 计算漫反射
	float diff = max(dot(norm, lightDir), 0.0);

	// 计算镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // 镜面反射需要的光线方向为光源指向片段位置
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// 环境光+漫反射+高光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	// 计算光线的衰减
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	vec3 result = specular + diffuse + ambient;

	result *= intensity;
	result *= attenuation;

	// 计算最终颜色
	FragColor = vec4(result, 1.0);


}