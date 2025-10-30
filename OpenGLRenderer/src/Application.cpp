#include "Application.h"
#include <corecrt_math_defines.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "math/Matrix3x3_f.h"
#include "math/Vector4f.h"



Application::Application(ApplicationSpecs appSpecs):
m_Window(appSpecs.windowSpecs)
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw std::runtime_error("Failed to initialize GLAD");
	}

	m_Window.SetViewport(0, 0, m_Window.GetWidth(), m_Window.GetHeight());
	m_Window.SetScrollCallback(scroll_callback);

	m_Program = std::make_unique<ShaderProgram>(appSpecs.vertexShaderPath, appSpecs.fragmentShaderPath);
	m_texture2d = std::make_unique<Texture2d>(appSpecs.texturePath);
	m_Model = std::make_unique<Model>(appSpecs.modelPath);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	m_Camera.SetFov(ToRadians(90.0f));
	m_Camera.SetNear(0.1f);
	m_Camera.SetFar(1.0f);

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
	m_Window.ProcessInput();
	m_Camera.ProcessInput();

	MouseInput::offsetX = 0.0f;
	MouseInput::offsetY = 0.0f;
}

void Application::Update()
{
	m_Camera.UpdateOrbitalPositionAndRotation(m_WorldTrans.GetPosition());
}

void Application::Render()
{
	GLuint mvpLocation = glGetUniformLocation(m_Program->GetId(), "mvp");
	GLuint mvLocation = glGetUniformLocation(m_Program->GetId(), "mv");
	GLuint mvNormLocation = glGetUniformLocation(m_Program->GetId(), "mvNorm");

	auto mv4x4 = m_Camera.GetViewMatrix() * m_WorldTrans.GetMatrix();

	auto mv3x3 = Matrix3x3_f{
		mv4x4.values[0], mv4x4.values[1], mv4x4.values[2],
		mv4x4.values[4], mv4x4.values[5], mv4x4.values[6],
		mv4x4.values[8], mv4x4.values[9], mv4x4.values[10],
	};

	auto mvForNormals = mv3x3.Inversed().Transposed();

	auto mvp = m_Camera.GetProjectionMatrix(static_cast<float>(m_Window.GetWidth()) / static_cast<float>(m_Window.GetHeight())) * mv4x4;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH);

	m_Program->Bind();
	glUniformMatrix4fv(mvpLocation, 1, GL_TRUE, mvp.values);
	glUniformMatrix4fv(mvLocation, 1, GL_TRUE, mv4x4.values);
	glUniformMatrix3fv(mvNormLocation, 1, GL_TRUE, mvForNormals.values);


	m_Model->Draw(*m_Program, *m_texture2d);


	m_Window.SwapBuffers();

	glfwPollEvents();
}

void Application::Destroy()
{
	glfwTerminate();
}


