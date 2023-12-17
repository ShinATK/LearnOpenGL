#version 450 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D vertexTexture0;
uniform sampler2D vertexTexture1;
uniform float show;

void main()
{
    FragColor = mix(texture(vertexTexture0, TexCoord), texture(vertexTexture1, TexCoord), show);
}