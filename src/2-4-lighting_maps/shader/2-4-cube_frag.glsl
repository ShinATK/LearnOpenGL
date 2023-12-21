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

in vec2 TexCoord;

uniform vec3 viewPos; // �۲���λ��
uniform float dynamic_texcoord;

void main()
{

	// ����������
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos); // ��������Ҫ�Ĺ��߷���ΪƬ��ָ���Դλ��
	float diff = max(dot(norm, lightDir), 0.0);

	// ���㾵�淴��
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // ���淴����Ҫ�Ĺ��߷���Ϊ��Դָ��Ƭ��λ��
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// ������+������+�߹�
	vec3 emission = vec3(texture(material.emission, vec2(TexCoord.x, TexCoord.y + dynamic_texcoord)));
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	// ����������ɫ
	FragColor = vec4(specular + diffuse + ambient + emission, 1.0);
}