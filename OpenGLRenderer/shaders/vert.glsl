#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 mvp;
uniform mat4 mv;
uniform mat3 mvNorm;

out vec4 MVPosition;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
	MVPosition = mv * vec4(aPos, 1.0f);
	Normal = mvNorm * aNormal;
	TexCoord = aTexCoord;
	gl_Position = mvp * vec4(aPos, 1.0f);
} 