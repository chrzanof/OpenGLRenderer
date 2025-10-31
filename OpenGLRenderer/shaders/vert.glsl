#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

out vec4 MVPosition;
out vec3 Normal;
out vec3 LightPos;
out vec2 TexCoord;

void main()
{
	TexCoord = aTexCoord;
	Normal = transpose(inverse(mat3(view * model))) * aNormal;
	MVPosition = view * model * vec4(aPos, 1.0f);
	LightPos = vec3(view * vec4(lightPos, 1.0f));
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
} 