#version 450 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;
layout (location=2) in vec2 aTex;

out vec4 vertexColor;
out vec2 TexCoord;

void main()
{
	vertexColor = vec4(aColor, 1.0);
	TexCoord = aTex;
	gl_Position = vec4(aPos, 1.0);
}