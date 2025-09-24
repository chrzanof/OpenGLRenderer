#pragma once
#include <string>
#include <glad/glad.h>

enum class ShaderType
{
	vertexShader = GL_VERTEX_SHADER,
	fragmentShader = GL_FRAGMENT_SHADER
};

class Shader
{
public:
	Shader(ShaderType type, const std::string& filePath);
	void Delete();
	GLuint GetId();
	const std::string& GetSource();

private:
	GLuint m_id;
	std::string m_source;
};

