#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(ShaderType type, const std::string& filePath)
{
	std::ifstream file;
	file.open(filePath);
	if (file.fail())
	{
		std::cout << "Error opening the file: " << filePath << "\n";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	m_source = buffer.str();

	m_id = glCreateShader(static_cast<GLenum>(type));
	const char* sourceCStr = m_source.c_str();
	glShaderSource(m_id, 1, &sourceCStr, NULL);
	glCompileShader(m_id);

	GLint shaderCompiled;
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &shaderCompiled);
	if (shaderCompiled != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(m_id, 1024, &log_length, message);
		std::cout << message;
	}
}

void Shader::Delete()
{
	glDeleteShader(m_id);
}

GLuint Shader::GetId() const
{
	return m_id;
}

const std::string& Shader::GetSource()
{
	return m_source;
}

