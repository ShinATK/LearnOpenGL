#version 450 core

// ��������Ĳ��ʽṹ��
struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

// �����Դ�Ĳ�ͬ������Ӱ��
struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

// ���յ���ɫ���
out vec4 FragColor;

// ������յģ������⡢�����䡢�߹���
in vec3 Normal; // ����
in vec3 FragPos; // Ƭ��λ��
in vec3 LightPos; // ��Դλ��

uniform vec3 viewPos; // �۲���λ��

void main()
{
	// ������
	vec3 ambient = light.ambient * material.ambient;

	// ������
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// �߹�
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	// ������ɫ
	vec3 result = specular + diffuse + ambient;
	FragColor = vec4(result, 1.0);
}