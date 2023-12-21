#version 450 core

// 片段颜色
out vec4 FragColor;
// 物体的材质坐标
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec4 View;

uniform vec3 viewPos;

// 定义物体的材质结构体
struct Material{
	sampler2D emission;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

struct Light{

	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float CutOff;
	float outerCutOff;
};
#define NR_POINT_LIGHTS 4
uniform Light dirLight, pointLight[NR_POINT_LIGHTS], spotLight;
uniform sampler2D spotLight_diffuseMap;

#define DIR_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

// 计算物体的着色
vec3 CalcLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, int light_type);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	// 定向光照
	vec3 result = CalcLight(dirLight, norm, FragPos, viewDir, DIR_LIGHT);
	// 点光源
	for(int i=0;i<NR_POINT_LIGHTS;i++)
		result += CalcLight(pointLight[i], norm, FragPos, viewDir, POINT_LIGHT);
	// 聚光
	result += CalcLight(spotLight, norm, FragPos, viewDir, SPOT_LIGHT);
	// 计算最终颜色
	FragColor = vec4(result, 1.0);

}

vec3 CalcLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, int light_type)
{
	vec3 lightDir;
	if(light_type == 0) // 定向光
		lightDir = normalize(-light.direction);
	else				// 非定向光：着色点指向光源
		lightDir = normalize(light.position - fragPos);

	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// 合并结果：定向光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
	if(light_type == 0)
		return (ambient + diffuse + specular);
	if(light_type == 1) // 点光源
	{
		float distance = length(light.position - FragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
		return (ambient + diffuse + specular)*attenuation;
	}
	if(light_type == 2) // 聚光
	{	
		vec2 texcoord = normalize(View.xyz).xy;
		vec3 spotdiffuse = light.diffuse * diff * texture(spotLight_diffuseMap, TexCoord).rgb;

		float theta = dot(lightDir, normalize(-light.direction));
		float epsilon = light.CutOff - light.outerCutOff;
		float intensity = clamp((theta-light.outerCutOff)/epsilon, 0.0, 1.0);

		return (ambient + diffuse + specular + spotdiffuse)*intensity;
	}
	return (ambient + diffuse + specular);
}
