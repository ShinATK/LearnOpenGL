#version 450 core

out vec4 FragColor;

in vec3 TexCoords;

uniform sampler2D skyboxTexture;


void main()
{ 	
	FragColor = texture(skyboxTexture, TexCoords);
}
