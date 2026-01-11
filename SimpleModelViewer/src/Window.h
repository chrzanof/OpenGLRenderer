#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "math/Vector2f.h"

struct WindowSpecs
{
	int width = 1920;
	int height = 1080;
	std::string title;
	int versionMajor = 3;
	int versionMinor = 3;
};
class Window
{
public:
	Window(WindowSpecs windowSpecs);
	~Window();
	void ProcessInput();
	int ShouldClose();
	void Terminate();

	void SetViewport(int x, int y, int width, int height);
	int GetWidth() const;
	int GetHeight() const;
	const std::string& GetTile() const;
	void SwapBuffers() const;
	void SetFrameBufferSizeCallback(GLFWframebuffersizefun callback) const;
	void SetScrollCallback(GLFWscrollfun callback) const;
	void SetDropCallback(GLFWdropfun callback) const;
	bool IsKeyPressed(int key) const;
	GLFWwindow* GetGLFWwindow() const;

private:

	bool IsLeftMouseButtonClicked() const;
	bool IsRightMouseButtonClicked() const;
	Vector2f GetCursorPosition() const;


	GLFWwindow* m_Window;
	int m_Width;
	int m_Height;
	std::string m_Title;
	int m_VersionMajor;
	int m_VersionMinor;
};





