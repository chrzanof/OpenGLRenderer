#pragma once
#include "Shader.h"
#include "math/Matrix4x4_f.h"
#include"math/Vector3f.h"

class ShaderProgram
{
public:
	ShaderProgram(Shader&& vertexShader, Shader&& fragmentShader);
	ShaderProgram(Shader&& vertexShader, Shader&& geometryShader, Shader&& fragmentShader);
	ShaderProgram(const std::string& vertexShaderFilePath,
				  const std::string& fragmentShaderFilePath);
	ShaderProgram(const std::string& vertexShaderFilePath,
				  const std::string& geometryShaderFilePath,
				  const std::string& fragmentShaderFilePath);

	GLuint GetId() const;
	void Bind() const;
	void Unbind() const;
	void SetMat4f(const std::string& name, const Matrix4x4_f& mat) const;
	void SetVec3f(const std::string& name, const Vector3f& vec) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
private:
	GLuint m_Id;
};

