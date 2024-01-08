#version 450 core

// 定义物体的材质结构体
struct Material{
	vec3 ambient;
	sampler2D diffuse;
	vec3 specular;
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
in vec2 TexCoords;

uniform vec3 viewPos; // 观察者位置
uniform int LightMethod;

void main()
{
	// 环境光
	vec3 ambient = light.ambient * material.ambient;

	// 漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);

	// 高光
	vec3 reflectDir = reflect(-lightDir, norm);

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfVec = normalize(lightDir + viewDir);

	float BlinnPhong = dot(halfVec, norm);
	float Phong = dot(reflectDir, viewDir);
	
	float final = (LightMethod==1)? BlinnPhong : Phong;

	float spec = pow(max(final, 0.0), material.shininess);

	vec3 specular = light.specular * (spec * material.specular);

	// 计算颜色
	vec3 result = specular + diffuse + ambient;

	FragColor = vec4(result, 1.0);
}