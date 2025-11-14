#include "Application.h"
#include <corecrt_math_defines.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "math/Matrix3x3_f.h"
#include "math/Vector4f.h"


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

	if (ImGui::Button("Open Folder..."))
	{
		ImGuiFileDialog::Instance()->OpenDialog(
			"ChooseFolder",     
			"Select Model Folder", 
			".obj,.fbx,.gltf"
		);

	}
	if (ImGuiFileDialog::Instance()->Display("ChooseFolder"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			
		}

		ImGuiFileDialog::Instance()->Close();
	}

	ImGui::End();
}

void Application::ReloadModelAndTexture(const std::string& modelPath, const std::string& texturePath)
{
}

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
	m_texture2d = std::make_unique<Texture2d>(appSpecs.texturePath);
	m_Model = std::make_unique<Model>(appSpecs.modelPath);

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

void Application::Run()
{
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);

	std::cout << "OpenGL Version: " << version << std::endl;
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "Vendor: " << vendor << std::endl;

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
	GLuint modelLocation = glGetUniformLocation(m_Program->GetId(), "model");
	GLuint viewLocation = glGetUniformLocation(m_Program->GetId(), "view");
	GLuint projectionLocation = glGetUniformLocation(m_Program->GetId(), "projection");
	GLuint lightPosLocation = glGetUniformLocation(m_Program->GetId(), "lightPos");

	auto model = m_WorldTrans.GetMatrix();
	auto view = m_Camera.GetViewMatrix();
	auto projection = m_Camera.GetProjectionMatrix(float(m_Window.GetWidth()) / float(m_Window.GetHeight()));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH);

	m_Program->Bind();
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, model.values);
	glUniformMatrix4fv(viewLocation, 1, GL_TRUE, view.values);
	glUniformMatrix4fv(projectionLocation, 1, GL_TRUE, projection.values);
	glUniform3fv(lightPosLocation, 1, &m_LightPos.x);


	m_Model->Draw(*m_Program, *m_texture2d);

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


