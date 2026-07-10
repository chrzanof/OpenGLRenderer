#include "Mesh.h"

#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
	std::vector<std::shared_ptr<Texture2d>> textures) :
m_Vertices(std::move(vertices)), m_Indices(std::move(indices)), m_Textures(std::move(textures)), m_VAO(0), m_VBO(0), m_EBO(0)
{
	SetupMesh();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_Vertices.size(), &m_Vertices.front(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_Indices.size(), &m_Indices.front(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3f));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vector3f)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(Vector3f)));

	glBindVertexArray(0);
}

const std::vector<Vertex>& Mesh::GetVerticesData() const
{
	return m_Vertices;
}

const std::vector<unsigned int>& Mesh::GetIndicesData() const
{
	return m_Indices;
}

void Mesh::Bind() const
{
	glBindVertexArray(m_VAO);
}

void Mesh::Unbind() const
{
	glBindVertexArray(0);
}

void Mesh::Draw(ShaderProgram& shaderProgram) const
{
	shaderProgram.Bind();
	if(!m_Textures.empty())
	{
		
		int textureIndex = 0;

		while (textureIndex < m_Textures.size() && m_Textures[textureIndex]->GetTypeName() != "texture_diffuse") textureIndex++;
		if (textureIndex < m_Textures.size())
		{
			glActiveTexture(GL_TEXTURE1);
			m_Textures[textureIndex]->Bind();
		}

		while (textureIndex < m_Textures.size() && m_Textures[textureIndex]->GetTypeName() != "texture_normal") textureIndex++;
		if (textureIndex < m_Textures.size())
		{
			glActiveTexture(GL_TEXTURE2);
			m_Textures[textureIndex]->Bind();
		}

	}
	this->Bind();
	glDrawElements(GL_TRIANGLES,  static_cast<GLsizei>(m_Indices.size()), GL_UNSIGNED_INT, 0);
	this->Unbind();
	if (!m_Textures.empty())
		m_Textures[0]->Unbind();
	shaderProgram.Unbind();
}
