#pragma once
#include "Shader.h"

class ShaderProgram
{
public:


	ShaderProgram(Shader&& vertexShader, Shader&& fragmentShader);

	ShaderProgram(const std::string& vertexShaderFilePath,
	              const std::string& fragmentShaderFilePath);

	GLuint GetId() const;
	void Use() const;

private:
	GLuint m_id;
};

