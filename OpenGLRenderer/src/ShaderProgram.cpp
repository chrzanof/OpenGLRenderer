#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(Shader&& vertexShader, Shader&& fragmentShader)
{
	this->m_id = glCreateProgram();
	glAttachShader(this->m_id, vertexShader.GetId());
	glAttachShader(this->m_id, fragmentShader.GetId());
	glLinkProgram(this->m_id);
	this->Use();
	vertexShader.Delete();
	fragmentShader.Delete();
}

ShaderProgram::ShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	:ShaderProgram(
		Shader(ShaderType::vertexShader, vertexShaderFilePath),
		Shader(ShaderType::fragmentShader,fragmentShaderFilePath)
	)
{
															
}

GLuint ShaderProgram::GetId() const
{
	return this->m_id;
}

void ShaderProgram::Use() const
{
	glUseProgram(this->m_id);
}
