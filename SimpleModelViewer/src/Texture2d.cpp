#include "Texture2d.h"

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>

Texture2d::Texture2d(const std::string& fileName)
{
	GLuint texture;
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data = stbi_load(fileName.c_str(), &m_width, &m_height, &m_nrChannels, 0);

	if (data)
	{
		GLint format = m_nrChannels == 4 ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture\n";
	}
	stbi_image_free(data);
	this->Unbind();
}

Texture2d::~Texture2d()
{
	delete m_data;
}

void Texture2d::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture2d::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture2d::GetId() const
{
	return m_id;
}

int Texture2d::GetWidth() const
{
	return m_width;
}

int Texture2d::GetHeight() const
{
	return m_height;
}

int Texture2d::GetNrChannels() const
{
	return m_nrChannels;
}

const unsigned char* Texture2d::GetData() const
{
	return m_data; 
}
