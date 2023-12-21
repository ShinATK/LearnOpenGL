#version 450 core

// ��������Ĳ��ʽṹ��
struct Material{
	sampler2D emission;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

// �����Դ�Ĳ�ͬ������Ӱ��
struct Light{

	vec3 position;
	vec3 direction;
	float CutOff;		//	���й��
	float outerCutOff;	// ���й��

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
uniform Light light;

// ���յ���ɫ���
out vec4 FragColor;

// ������յģ������⡢�����䡢�߹���
in vec3 Normal; // ����
in vec3 FragPos; // Ƭ��λ��

in vec2 TexCoord;

uniform vec3 viewPos; // �۲���λ��

void main()
{
	// ����
	vec3 norm = normalize(Normal);

	// �����Դ����
	vec3 lightDir = normalize(light.position - FragPos);

	// ģ���ֵ�Ͳ�۹�
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.CutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0);

	// ����������
	float diff = max(dot(norm, lightDir), 0.0);

	// ���㾵�淴��
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // ���淴����Ҫ�Ĺ��߷���Ϊ��Դָ��Ƭ��λ��
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// ������+������+�߹�
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	// ������ߵ�˥��
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	vec3 result = specular + diffuse + ambient;

	result *= intensity;
	result *= attenuation;

	// ����������ɫ
	FragColor = vec4(result, 1.0);


}