#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <vector>

#include "Vertex.h"

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void setupMesh();
	const std::vector<Vertex>& GetVerticesData() const;
	const std::vector<unsigned int>& GetIndicesData() const;
	void Bind() const;
	void UnbindVAO() const;

private:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;

	GLuint m_VAO, m_VBO, m_EBO;
};

