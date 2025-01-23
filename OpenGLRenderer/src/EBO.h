#pragma once

#include <glad/glad.h>
#include <vector>

class EBO
{
public:
	unsigned int ID;
	EBO()
	{
		glGenBuffers(1, &ID);
	}

	void Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	}

	void SetData(const std::vector<unsigned int>& indices)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,  indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);
	}

};
