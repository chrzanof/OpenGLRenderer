#pragma once

#include <glad/glad.h>
#include <vector>

class VBO
{
public:
	unsigned int ID;
	VBO()
	{
		glGenBuffers(1, &ID);
	}

	void Bind()
	{
		glBindBuffer(GL_VERTEX_ARRAY, ID);
	}

	void SetData(const std::vector<float>& vertices, unsigned int length)
	{
		
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
	}

};
