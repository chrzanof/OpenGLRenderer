#pragma once

#include <glad/glad.h>

class VAO
{
public:
	unsigned int ID;
	VAO()
	{
		glGenVertexArrays(1, &ID);
	}
	void Bind()
	{
		glBindVertexArray(ID);
	}
	void SetAttributePointer(unsigned int index, GLint size, GLenum type, GLsizei stride, unsigned int offset)
	{
		glVertexAttribPointer(index, size, type, GL_FALSE, stride * sizeof(type), (void*)(offset * sizeof(type)));
		glEnableVertexAttribArray(index);
	}
};
