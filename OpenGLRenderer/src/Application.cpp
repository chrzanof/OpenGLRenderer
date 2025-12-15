#include "Application.h"
#include <corecrt_math_defines.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "math/Matrix3x3_f.h"
#include "math/Vector4f.h"

Application::Application(ApplicationSpecs appSpecs):
m_Window(appSpecs.windowSpecs), m_LightPos(10.0f, 1.0f, -1.0f)
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw std::runtime_error("Failed to initialize GLAD");
	}

	m_Window.SetViewport(0, 0, m_Window.GetWidth(), m_Window.GetHeight());
	m_Window.SetScrollCallback(scroll_callback);

	m_Program = std::make_unique<ShaderProgram>(appSpecs.vertexShaderPath, appSpecs.fragmentShaderPath);
	m_TexturePathName = "models/wooden_crate.jpg";
	m_Texture2d = std::make_unique<Texture2d>(m_TexturePathName.string());
	m_ModelPathName = "models/cube.obj";
	m_Model = std::make_unique<Model>(m_ModelPathName.string());

	InitImGui(m_Window.GetGLFWwindow());

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

void Application::InitImGui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	// GLSL version for OpenGL 3.3
	const char* glsl_version = "#version 330";

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void Application::ShutdownImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Application::DrawImGui()
{
	ImGui::Begin("Model Viewer Controls");

	ImGui::TextWrapped("Model: %s", m_ModelPathName.filename().string().c_str());
	if (ImGui::Button("Choose model..."))
	{
		ImGuiFileDialog::Instance()->OpenDialog(
			"ChooseModel",
			"Select Model Location",
			".*"
		);

	}
	ImGui::TextWrapped("Texture: %s", m_TexturePathName.filename().string().c_str());
	if (ImGui::Button("Choose texture..."))
	{
		ImGuiFileDialog::Instance()->OpenDialog(
			"ChooseTexture",
			"Select Texture Location",
			".*"
		);

	}
	ImGui::TextWrapped("");
	ImGui::TextWrapped("Rotate: LMB + Drag");
	ImGui::TextWrapped("Zoom: Mouse Wheel");
	if (ImGuiFileDialog::Instance()->Display("ChooseModel"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			m_ModelPathName = ImGuiFileDialog::Instance()->GetFilePathName();

			if (!std::filesystem::is_directory(m_ModelPathName))
			{
				m_Model = nullptr;
				m_Model = std::make_unique<Model>(m_ModelPathName.string());
			}
		}

		ImGuiFileDialog::Instance()->Close();
	}
	if (ImGuiFileDialog::Instance()->Display("ChooseTexture"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			m_TexturePathName = ImGuiFileDialog::Instance()->GetFilePathName();

			if (!std::filesystem::is_directory(m_TexturePathName))
			{
				m_Texture2d = nullptr;
				m_Texture2d = std::make_unique<Texture2d>(m_TexturePathName.string());
			}
		}

		ImGuiFileDialog::Instance()->Close();
	}

	ImGui::End();
}

void Application::Run()
{
	while(!m_Window.ShouldClose())
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ProcessInput();
		Update();
		Render();
	}
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
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	GLuint modelLocation = glGetUniformLocation(m_Program->GetId(), "model");
	GLuint viewLocation = glGetUniformLocation(m_Program->GetId(), "view");
	GLuint projectionLocation = glGetUniformLocation(m_Program->GetId(), "projection");
	GLuint lightPosLocation = glGetUniformLocation(m_Program->GetId(), "lightPos");

	auto model = m_WorldTrans.GetMatrix();
	auto view = m_Camera.GetViewMatrix();
	auto projection = m_Camera.GetProjectionMatrix(float(m_Window.GetWidth()) / float(m_Window.GetHeight()));
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH);
	m_Program->Bind();
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, model.values);
	glUniformMatrix4fv(viewLocation, 1, GL_TRUE, view.values);
	glUniformMatrix4fv(projectionLocation, 1, GL_TRUE, projection.values);
	glUniform3fv(lightPosLocation, 1, &m_LightPos.x);

	if (m_Texture2d && m_Model)
	{
		m_Model->Draw(*m_Program, *m_Texture2d);
	}


	DrawImGui();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	m_Window.SwapBuffers();

	glfwPollEvents();
}

void Application::Destroy()
{
	glfwTerminate();
}


