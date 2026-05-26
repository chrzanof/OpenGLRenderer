#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(Shader&& vertexShader, Shader&& fragmentShader)
{
	this->m_Id = glCreateProgram();
	glAttachShader(this->m_Id, vertexShader.GetId());
	glAttachShader(this->m_Id, fragmentShader.GetId());
	glLinkProgram(this->m_Id);
	this->Bind();
	vertexShader.Delete();
	fragmentShader.Delete();
	this->Unbind();
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
	return this->m_Id;
}

void ShaderProgram::Bind() const
{
	glUseProgram(this->m_Id);
}

void ShaderProgram::Unbind() const
{
	glUseProgram(0);
}

void ShaderProgram::SetMat4f(const std::string& name, const Matrix4x4_f& mat) const
{
	GLuint location = glGetUniformLocation(m_Id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_TRUE, mat.values);
}

void ShaderProgram::SetVec3f(const std::string& name, const Vector3f& vec) const
{
	GLuint location = glGetUniformLocation(m_Id, name.c_str());
	glUniform3fv(location, 1, &vec.x);
}

void ShaderProgram::SetInt(const std::string& name, int value) const
{
	GLuint location = glGetUniformLocation(m_Id, name.c_str());
	glUniform1i(location, value);
}

void ShaderProgram::SetFloat(const std::string& name, float value) const
{
	GLuint location = glGetUniformLocation(m_Id, name.c_str());
	glUniform1f(location, value);
}
