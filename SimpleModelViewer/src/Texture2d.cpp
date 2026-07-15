#include "Texture2d.h"

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <filesystem>

Texture2d::Texture2d(const std::string& fileName, const std::string& typeName)
{
	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_2D, m_Id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data = stbi_load(fileName.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);

	if (data)
	{
		GLint format = m_NrChannels == 4 ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture from path: " << fileName <<"\n";
	}
	std::filesystem::path path = std::filesystem::path(fileName);
	if (!std::filesystem::exists(path))
	{
		std::cout << "Path to texture doesn't exist!\n";
	}
	stbi_image_free(data);
	m_TypeName = typeName;
	this->Unbind();
}

Texture2d::Texture2d(const aiTexture* aiTexture, const std::string& typeName)
{
	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_2D, m_Id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if(!aiTexture)
	{
		std::cout << "aiTexture is null!\n";
		return;
	}

	if (aiTexture->mHeight)
	{
		GLint format = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, (int)aiTexture->mWidth, (int)aiTexture->mHeight, 0, format, GL_UNSIGNED_BYTE, aiTexture->pcData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		unsigned char* data = stbi_load_from_memory((unsigned char*)aiTexture->pcData, aiTexture->mWidth, &m_Width, &m_Height, &m_NrChannels, 0);
		if (data)
		{
			GLint format = m_NrChannels == 4 ? GL_RGBA : GL_RGB;
			glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	m_TypeName = typeName;
	this->Unbind();
}

void Texture2d::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_Id);
}

void Texture2d::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture2d::GetId() const
{
	return m_Id;
}

int Texture2d::GetWidth() const
{
	return m_Width;
}

int Texture2d::GetHeight() const
{
	return m_Height;
}

int Texture2d::GetNrChannels() const
{
	return m_NrChannels;
}

const std::string& Texture2d::GetTypeName() const
{
	return m_TypeName;
}
