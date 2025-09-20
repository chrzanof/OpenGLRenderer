#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform float gScale;
uniform mat4 transform;

out vec4 Color;
out vec2 TexCoord;

void main()
{
	gl_Position = transform * vec4(aPos.x * gScale, aPos.y * gScale, aPos.z * gScale, 1.0f);
	Color = aColor;
	TexCoord = aTexCoord;
} 