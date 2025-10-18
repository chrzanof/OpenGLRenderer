#include "Application.h"

#include <corecrt_math_defines.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
Application::Application(ApplicationSpecs appSpecs):
m_Window(appSpecs.windowSpecs)
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw std::runtime_error("Failed to initialize GLAD");
	}

	m_Window.SetViewport(0, 0, m_Window.GetWidth(), m_Window.GetHeight());

	m_Program = std::make_unique<ShaderProgram>("shaders/vert.glsl", "shaders/frag.glsl");
	m_texture2d = std::make_unique<Texture2d>("models/bear-head/textures/PM3D_Sphere3D_1DiffuseMap.tga.png");
	m_Model = std::make_unique<Model>(appSpecs.modelPath);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	m_Camera.SetFov(ToRadians(90.0f));
	m_Camera.SetNear(0.1f);
	m_Camera.SetFar(1.0f);
	m_Camera.SetWidth(m_Window.GetWidth());
	m_Camera.SetHeight(m_Window.GetHeight());

	m_WorldTrans.SetPosition(0.0f, 0.0f, 0.0f);
	m_WorldTrans.SetRotation(0.0f, 0.0f, 0.0f);
	m_WorldTrans.SetScale(1.0f);
}

Application::~Application()
{
	Destroy();
}

void Application::Run()
{
	while(!m_Window.ShouldClose())
	{
		ProcessInput();
		Update();
		Render();
	}
}

void Application::Setup()
{
}

void Application::ProcessInput()
{
	if (m_Window.IsKeyPressed(GLFW_KEY_LEFT))
	{
		theta += 0.5f;
	}
	if (m_Window.IsKeyPressed(GLFW_KEY_RIGHT))
	{
		theta -= 0.5f;
	}
	if (m_Window.IsKeyPressed(GLFW_KEY_UP))
	{
		fi += 0.5f;
	}
	if (m_Window.IsKeyPressed(GLFW_KEY_DOWN))
	{
		fi -= 0.5f;
	}

	if (m_Window.IsKeyPressed(GLFW_KEY_MINUS))
	{
		r += 0.01f;
	}
	if (m_Window.IsKeyPressed(GLFW_KEY_EQUAL))
	{
		r -= 0.01f;
	}
	m_Window.ProcessInput();
}

void Application::Update()
{
	m_Camera.SetTarget(0.0f, 0.0f, 0.0f);
	m_Camera.SetPositionFromSphericalCoords(ToRadians(theta), ToRadians(fi), r, { 0.0f, 0.0f, 0.0f });
}

void Application::Render()
{
	GLuint transformLocation = glGetUniformLocation(m_Program->GetId(), "transform");

	auto finalTransform = m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix() * m_WorldTrans.GetMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH);

	m_Program->Bind();
	glUniformMatrix4fv(transformLocation, 1, GL_TRUE, finalTransform.values);


	m_Model->Draw(*m_Program, *m_texture2d);


	m_Window.SwapBuffers();

	glfwPollEvents();
}

void Application::Destroy()
{
	glfwTerminate();
}


