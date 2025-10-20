
#include "Window.h"

Window::Window(WindowSpecs windowSpecs)
{
	m_Width = windowSpecs.width;
	m_Height = windowSpecs.height;
	m_Title = windowSpecs.title;
	
	if (!glfwInit())
	{
		std::cout << "Error initializing glfw.\n";
		return;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, windowSpecs.versionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, windowSpecs.versionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);

	if (!m_Window)
	{
		std::cout << "Error creating window.\n";
		Terminate();
		return;
	}
	glfwMakeContextCurrent(m_Window);
}

Window::~Window()
{
	Terminate();
}

void Window::ProcessInput()
{
	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
	}
}

int Window::ShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

void Window::Terminate()
{
	glfwTerminate();
}

void Window::SwapBuffers() const
{
	glfwSwapBuffers(m_Window);
}

void Window::SetFrameBufferSizeCallback(GLFWframebuffersizefun callback) const
{
	glfwSetFramebufferSizeCallback(m_Window, callback);
}

void Window::SetScrollCallback(GLFWscrollfun callback) const
{
	glfwSetScrollCallback(m_Window, callback);
}

bool Window::IsKeyPressed(int key) const
{
	int state = glfwGetKey(m_Window, key);
	return state == GLFW_PRESS;
}

bool Window::IsLeftMouseButtonClicked() const
{
	int state = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);
	return state == GLFW_PRESS;
}

bool Window::IsRightMouseButtonClicked() const
{
	int state = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT);
	return state == GLFW_PRESS;
}

Vector2f Window::GetCursorPosition() const
{
	double xpos, ypos;
	glfwGetCursorPos(m_Window, &xpos, &ypos);
	return Vector2f{ static_cast<float>(xpos), static_cast<float>(ypos)};
}

void Window::SetViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}



int Window::GetWidth() const
{
	return m_Width;
}

int Window::GetHeight() const
{
	return m_Height;
}

const std::string& Window::GetTile() const
{
	return m_Title;
}
