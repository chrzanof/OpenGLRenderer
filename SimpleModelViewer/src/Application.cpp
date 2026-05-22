#include "Application.h"
#include <corecrt_math_defines.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Input.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "math/Matrix3x3_f.h"
#include "math/Vector4f.h"

Application::Application(const ApplicationSpecs& appSpecs):
m_Window(appSpecs.windowSpecs), m_LightPos(10.0f, 1.0f, -1.0f), m_LightColor(1.0f, 1.0f, 1.0f)
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw std::runtime_error("Failed to initialize GLAD");
	}

	m_Window.SetViewport(0, 0, m_Window.GetWidth(), m_Window.GetHeight());
	m_Window.SetScrollCallback(ScrollCallback);
	m_Window.SetDropCallback(DropCallback);
	m_Window.SetFrameBufferSizeCallback(FramebufferSizeCallback);

	m_ModelShader = std::make_unique<ShaderProgram>(appSpecs.vertexShaderPath, appSpecs.fragmentShaderPath);
	m_ModelPath = appSpecs.defaultModelPath;
	m_TexturePath = appSpecs.defaultTexturePath;
	m_Model = std::make_unique<Model>(m_ModelPath.string());
	m_Model->AddTexture(m_TexturePath.string());

	m_SkyboxShader = std::make_unique<ShaderProgram>("shaders/envVert.glsl", "shaders/envFrag.glsl");

	m_Skybox = std::make_unique<Skybox>(appSpecs.skyboxFaces);

	InitImGui(m_Window.GetGLFWwindow());

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	m_WorldTrans.SetPosition(0.0f, 0.0f, 0.0f);
	m_WorldTrans.SetRotation(0.0f, 0.0f, 0.0f);
	m_WorldTrans.SetScale(1.0f);

	m_Camera.SetFov(TO_RADIANS(90.0f));
	m_Camera.SetNearPlane(0.1f);
	m_Camera.SetFarPlane(1.0f);
	m_Camera.FocusOn(*m_Model, m_WorldTrans);
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

	ImGui::TextWrapped("Model: %s", m_ModelPath.filename().string().c_str());
	if (ImGui::Button("Choose model..."))
	{
		ImGuiFileDialog::Instance()->OpenDialog(
			"ChooseModel",
			"Select Model Location",
			".*"
		);

	}
	ImGui::TextWrapped("Texture: %s", m_TexturePath.filename().string().c_str());
	if (ImGui::Button("Choose texture..."))
	{
		ImGuiFileDialog::Instance()->OpenDialog(
			"ChooseTexture",
			"Select Texture Location",
			".*"
		);

	}
	ImGui::TextWrapped("or drop model & texture files anywhere in the window");
	ImGui::TextWrapped("");
	if (ImGuiFileDialog::Instance()->Display("ChooseModel"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			m_ModelPath = ImGuiFileDialog::Instance()->GetFilePathName();

			if (!std::filesystem::is_directory(m_ModelPath))
			{
				m_Model = nullptr;
				m_Model = std::make_unique<Model>(m_ModelPath.string());
				m_LightPosLimit = m_Model->GetLargestDiagonal().Length() * 10.0f;
			}
		}

		ImGuiFileDialog::Instance()->Close();
	}
	if (ImGuiFileDialog::Instance()->Display("ChooseTexture"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			m_TexturePath = ImGuiFileDialog::Instance()->GetFilePathName();

			if (!std::filesystem::is_directory(m_TexturePath))
			{
				m_Model->AddTexture(m_TexturePath.string());
			}
		}

		ImGuiFileDialog::Instance()->Close();
	}

	ImGui::SliderFloat3("Light Position", &m_LightPos.x, -m_LightPosLimit, m_LightPosLimit);
	ImGui::SliderFloat3("Light Color", &m_LightColor.x, 0.0f, 1.f);
	ImGui::TextWrapped("Rotate: LMB + Drag");
	ImGui::TextWrapped("Zoom: Mouse Wheel");

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

	MouseInput::s_OffsetX = 0.0f;
	MouseInput::s_OffsetY = 0.0f;
}

void Application::Update()
{
	if(s_DroppedModelPath != "" && s_DroppedModelPath != m_ModelPath)
	{
		m_ModelPath = s_DroppedModelPath;
		m_Model.reset();
		m_Model = std::make_unique<Model>(m_ModelPath.string());
		m_Camera.FocusOn(*m_Model, m_WorldTrans);
		m_LightPosLimit = m_Model->GetLargestDiagonal().Length() * 10.0f;
	}
	if(s_DroppedTexturePath != "" && s_DroppedTexturePath != m_TexturePath)
	{
		m_TexturePath = s_DroppedTexturePath;
		m_Model->AddTexture(m_TexturePath.string());
	}
	m_Camera.UpdateOrbitalPositionAndRotation();
}

void Application::Render()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	auto model = m_WorldTrans.GetMatrix();
	auto view = m_Camera.GetViewMatrix();
	auto projection = m_Camera.GetProjectionMatrix(float(m_Window.GetWidth()) / float(m_Window.GetHeight()));
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	m_ModelShader->Bind();

	m_ModelShader->SetMat4f("model", model);
	m_ModelShader->SetMat4f("view", view);
	m_ModelShader->SetMat4f("projection", projection);

	m_ModelShader->SetVec3f("lightPos", m_LightPos);
	m_ModelShader->SetVec3f("lightColor", m_LightColor);

	if (m_Model)
	{
		m_Model->Draw(*m_ModelShader);
	}
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	
	m_SkyboxShader->Bind();



	m_SkyboxShader->SetMat4f("view", view);
	m_SkyboxShader->SetMat4f("projection", projection);

	m_Skybox->Draw(*m_SkyboxShader);
	
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

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

void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	MouseInput::s_OffsetX = static_cast<float>(xoffset);
	MouseInput::s_OffsetY = static_cast<float>(yoffset);
}

void Application::DropCallback(GLFWwindow* window, int count, const char** paths)
{

	for (int i = 0; i < count; i++)
	{
		std::filesystem::path droppedFilePath(paths[i]);
		std::string extension = droppedFilePath.extension().string();

		if (std::find(s_TextureExtensions.begin(), s_TextureExtensions.end(), extension) != s_TextureExtensions.end())
		{
			s_DroppedTexturePath = droppedFilePath;
			continue;
		}

		if (std::find(s_AssimpExtensions.begin(), s_AssimpExtensions.end(), extension) != s_AssimpExtensions.end())
		{
			s_DroppedModelPath = droppedFilePath;
		}
	}

}

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	if(Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window)))
	{
		windowPtr->OnResize(width, height);
	}
}
