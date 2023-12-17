#version 450 core

out vec4 FragColor;

in vec3 vertexColor;
in vec3 TexCoord;

uniform sampler2D vertexTexture0;
uniform sampler2D vertexTexture1;

void main()
{
    FragColor = mix(texture(vertexTexture0, vec2(TexCoord.x,TexCoord.z)), texture(vertexTexture1, vec2(TexCoord.y,TexCoord.z)), 0.2);
}