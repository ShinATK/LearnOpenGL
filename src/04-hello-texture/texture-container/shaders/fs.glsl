#version 450 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D vertexTexture;

void main()
{
    FragColor = texture(vertexTexture, TexCoord);
}