#pragma once
#include <string>
#include <assimp/texture.h>

class Texture2d
{
public:
	Texture2d(const std::string& fileName, const std::string& typeName);
	Texture2d(const aiTexture* aiTexture, const std::string& typeName);
	void Bind() const;
	void Unbind() const;
	unsigned int GetId() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetNrChannels() const;
	const std::string& GetTypeName() const;

private:
	unsigned int m_Id;
	int m_Width, m_Height, m_NrChannels;
	std::string m_TypeName;
};

