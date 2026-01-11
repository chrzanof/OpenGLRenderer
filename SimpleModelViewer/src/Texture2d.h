#pragma once
#include <string>

class Texture2d
{
public:
	Texture2d(const std::string& fileName);
	~Texture2d();

	void Bind() const;
	void Unbind() const;
	unsigned int GetId() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetNrChannels() const;
	const unsigned char* GetData() const;

private:
	unsigned int m_id;
	int m_width, m_height, m_nrChannels;
	unsigned char* m_data;
};

