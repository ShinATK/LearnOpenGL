#version 450 core

// ��������Ĳ��ʽṹ��
struct Material{
	vec3 ambient;
	sampler2D diffuse;
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
in vec2 TexCoords;

uniform vec3 viewPos; // �۲���λ��
uniform int LightMethod;

void main()
{
	// ������
	vec3 ambient = light.ambient * material.ambient;

	// ������
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);

	// �߹�
	vec3 reflectDir = reflect(-lightDir, norm);

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfVec = normalize(lightDir + viewDir);

	float BlinnPhong = dot(halfVec, norm);
	float Phong = dot(reflectDir, viewDir);
	
	float final = (LightMethod==1)? BlinnPhong : Phong;

	float spec = pow(max(final, 0.0), material.shininess);

	vec3 specular = light.specular * (spec * material.specular);

	// ������ɫ
	vec3 result = specular + diffuse + ambient;

	FragColor = vec4(result, 1.0);
}