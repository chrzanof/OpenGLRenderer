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
m_Window(appSpecs.windowSpecs), m_LightPos(0.0f, 1.0f, -1.0f), m_LightColor(1.0f, 1.0f, 1.0f)
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
	m_NormalTexturePath = appSpecs.defaultNormalMapPath;
	m_HeightTexturePath = appSpecs.defaultHeightMapPath;
	m_Model = std::make_unique<Model>(m_ModelPath.string(), flipUVs);
	m_Quad = std::make_unique<Quad>();
	m_Model->AddTexture(m_TexturePath.string(), "texture_diffuse");
	m_Model->AddTexture(m_NormalTexturePath.string(), "texture_normal");
	m_Model->AddTexture(m_HeightTexturePath.string(), "texture_height");
	normalMagnitude = m_Model->GetLargestDiagonal().Length() / 100.0f;

	m_SkyboxShader = std::make_unique<ShaderProgram>("shaders/envVert.glsl", "shaders/envFrag.glsl");

	m_Skybox = std::make_unique<Skybox>(appSpecs.skyboxFaces);

	m_SimpleDepthShader = std::make_unique<ShaderProgram>("shaders/simpleDepthVert.glsl", "shaders/simpleDepthFrag.glsl");

	m_DepthMapPreviewShader = std::make_unique<ShaderProgram>("shaders/debugDepthVert.glsl", "shaders/debugDepthFrag.glsl");
	m_LightSourceShader = std::make_unique<ShaderProgram>("shaders/lightSourceVert.glsl", "shaders/lightSourceFrag.glsl");
	m_ShowNormalsShader = std::make_unique<ShaderProgram>("shaders/showNormalsVert.glsl", "shaders/showNormalsGeom.glsl", "shaders/showNormalsFrag.glsl");
	m_WireframeShader = std::make_unique<ShaderProgram>("shaders/showNormalsVert.glsl", "shaders/wireframeGeom.glsl", "shaders/showNormalsFrag.glsl");

	InitImGui(m_Window.GetGLFWwindow());

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	m_ModelTrans.SetPosition(0.0f, 0.0f, 0.0f);
	m_ModelTrans.SetRotation(0.0f, 0.0f, 0.0f);
	m_ModelTrans.SetScale(1.0f);

	auto boundingBox = m_Model->GetBoundingBox();
	auto largestDiagonal = boundingBox.max - boundingBox.min;
	float modelSize = largestDiagonal.Length();
	m_QuadTrans.SetPosition(0.0f, boundingBox.min.y, 0.0f);
	m_QuadTrans.SetRotation(TO_RADIANS(-90.0f), 0.0f, 0.0f);
	m_QuadTrans.SetScale(10.0f * modelSize);

	m_LightSourceTrans.SetPosition(m_LightPos.x, m_LightPos.y, m_LightPos.z);
	m_LightSourceTrans.SetRotation(0.0f, 0.0f, 0.0f);
	m_LightSourceTrans.SetScale(0.05f);

	m_MainCamera.SetFov(TO_RADIANS(90.0f));
	m_MainCamera.SetNearPlane(0.1f);
	m_MainCamera.SetFarPlane(1.0f);
	m_MainCamera.SetAzimuth(45.0f);
	m_MainCamera.SetElevation(45.0f);
	m_MainCamera.FocusOn(*m_Model, m_ModelTrans);

	m_LightViewCamera.SetPosition(m_LightPos.x, m_LightPos.y, m_LightPos.z);
	m_LightViewCamera.SetFov(TO_RADIANS(90.f));
	m_LightViewCamera.SetNearPlane(0.1f);
	m_LightViewCamera.SetFarPlane(1.0f);
	m_LightViewCamera.LookAt(0.0f, 0.0f, 0.0f);

	glGenFramebuffers(1, &m_DepthMapFBO);

	glGenTextures(1, &m_DepthMap);
	glBindTexture(GL_TEXTURE_2D, m_DepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	create_framebuffer();
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
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport();
	ImGui::Begin("Model Viewer Controls");

	ImGui::TextWrapped("Model: %s", m_ModelPath.filename().string().c_str());
	ImGui::TextWrapped("Texture: %s", m_TexturePath.filename().string().c_str());
	ImGui::TextWrapped("");

	ImGui::SliderFloat3("Light Position", &m_LightPos.x, -m_LightPosLimit, m_LightPosLimit);
	ImGui::SliderFloat3("Light Color", &m_LightColor.x, 0.0f, 1.f);
	ImGui::TextWrapped("");

	ImGui::TextWrapped("Rotate: LMB + Drag");
	ImGui::TextWrapped("Zoom: Mouse Wheel");
	ImGui::TextWrapped("");

	ImGui::Checkbox("flip UVs", &flipUVs);
	ImGui::Checkbox("show Model", &showModel);
	ImGui::Checkbox("show normals", &showNormals);
	ImGui::Checkbox("show wireframe", &showWireframe);
	ImGui::Checkbox("use diffuse texture", &showDiffuseTexture);
	ImGui::Checkbox("use normal map", &showNormalMap);
	ImGui::Checkbox("use height map", &showHeightMap);

	ImGui::End();

	ImGui::Begin("Model View");

	m_ModelViewWidth = ImGui::GetContentRegionAvail().x;
	m_ModelViewHeight = ImGui::GetContentRegionAvail().y;

	glViewport(0, 0, m_ModelViewWidth, m_ModelViewHeight);


	m_ModelViewWindowPos = ImGui::GetCursorScreenPos();


	ImGui::GetWindowDrawList()->AddImage(
		(void*)texture_id,
		ImVec2(m_ModelViewWindowPos.x, m_ModelViewWindowPos.y),
		ImVec2(m_ModelViewWindowPos.x + m_ModelViewWidth, m_ModelViewWindowPos.y + m_ModelViewHeight),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::DrawScene()
{
	m_Window.SetViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	auto model = m_ModelTrans.GetMatrix();
	auto modelQuad = m_QuadTrans.GetMatrix();
	auto lightView = m_LightViewCamera.GetViewMatrix();
	auto lightProjection = m_LightViewCamera.GetProjectionMatrix(float(SHADOW_MAP_WIDTH) / float(SHADOW_MAP_HEIGHT));
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	m_SimpleDepthShader->Bind();
	m_SimpleDepthShader->SetMat4f("model", model);
	m_SimpleDepthShader->SetMat4f("lightView", lightView);
	m_SimpleDepthShader->SetMat4f("lightProjection", lightProjection);

	if (m_Model)
	{
		m_Model->Draw(*m_SimpleDepthShader);
	}
	m_SimpleDepthShader->Bind();
	m_SimpleDepthShader->SetMat4f("model", modelQuad);
	m_SimpleDepthShader->SetMat4f("lightView", lightView);
	m_SimpleDepthShader->SetMat4f("lightProjection", lightProjection);

	m_Quad->Draw(*m_SimpleDepthShader);

	m_Window.SetViewport(0, 0, m_ModelViewWidth, m_ModelViewHeight);
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	bind_framebuffer();

	auto view = m_MainCamera.GetViewMatrix();
	auto projection = m_MainCamera.GetProjectionMatrix(m_ModelViewWidth / m_ModelViewHeight);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	m_ModelShader->Bind();

	m_ModelShader->SetMat4f("model", model);
	m_ModelShader->SetMat4f("view", view);
	m_ModelShader->SetMat4f("projection", projection);
	m_ModelShader->SetMat4f("lightView", lightView);
	m_ModelShader->SetMat4f("lightProjection", lightProjection);

	m_ModelShader->SetVec3f("lightPos", m_LightPos);
	m_ModelShader->SetVec3f("lightColor", m_LightColor);
	m_ModelShader->SetInt("depthMap", 0);
	m_ModelShader->SetInt("diffuseTexture", 1);
	m_ModelShader->SetInt("normalMap", 2);
	m_ModelShader->SetInt("heightMap", 3);
	m_ModelShader->SetInt("useDiffuseTexture", showDiffuseTexture);
	m_ModelShader->SetInt("useNormalMap", showNormalMap);
	m_ModelShader->SetInt("useHeightMap", showHeightMap);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_DepthMap);

	if (m_Model && showModel)
	{
		m_Model->Draw(*m_ModelShader);
	}
	m_ModelShader->Bind();
	m_ModelShader->SetMat4f("model", modelQuad);
	m_ModelShader->SetMat4f("view", view);
	m_ModelShader->SetMat4f("projection", projection);

	m_ModelShader->SetVec3f("lightPos", m_LightPos);
	m_ModelShader->SetVec3f("lightColor", m_LightColor);
	m_ModelShader->SetInt("depthMap", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_Quad->Draw(*m_ModelShader);

	auto lightSourceModel = m_LightSourceTrans.GetMatrix();
	m_LightSourceShader->Bind();
	m_LightSourceShader->SetMat4f("model", lightSourceModel);
	m_LightSourceShader->SetMat4f("view", view);
	m_LightSourceShader->SetMat4f("projection", projection);
	m_LightSourceShader->SetVec3f("lightColor", m_LightColor);
	m_Model->Draw(*m_LightSourceShader);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	m_SkyboxShader->Bind();



	m_SkyboxShader->SetMat4f("view", view);
	m_SkyboxShader->SetMat4f("projection", projection);

	m_Skybox->Draw(*m_SkyboxShader);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	//glDisable(GL_CULL_FACE);
	//m_DepthMapPreviewShader->Bind();
	//m_DepthMapPreviewShader->SetFloat("near_plane", 0.1f);
	//m_DepthMapPreviewShader->SetFloat("far_plane", 1.0f);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_DepthMap);
	//renderQuad();
	//glEnable(GL_CULL_FACE);

	if (showNormals)
	{
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);
		m_ShowNormalsShader->Bind();
		m_ShowNormalsShader->SetMat4f("model", model);
		m_ShowNormalsShader->SetMat4f("view", view);
		m_ShowNormalsShader->SetMat4f("projection", projection);
		m_ShowNormalsShader->SetFloat("MAGNITUDE", normalMagnitude);
		m_Model->Draw(*m_ShowNormalsShader);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}

	if (showWireframe)
	{
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);
		m_WireframeShader->Bind();
		m_WireframeShader->SetMat4f("model", model);
		m_WireframeShader->SetMat4f("view", view);
		m_WireframeShader->SetMat4f("projection", projection);

		m_Model->Draw(*m_WireframeShader);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}
	unbind_framebuffer();
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

void Application::ProcessInput()
{
	m_Window.ProcessInput();
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
	{
		float mouseX = MouseInput::s_Position.x;
		float mouseY = MouseInput::s_Position.y;
		bool mouseXInsideWindow = mouseX > m_ModelViewWindowPos.x && mouseX < m_ModelViewWindowPos.x + m_ModelViewWidth;
		bool mouseYInsideWindow = mouseY > m_ModelViewWindowPos.y && mouseY < m_ModelViewWindowPos.y + m_ModelViewHeight;
		if (mouseXInsideWindow && mouseYInsideWindow) {
			m_MainCamera.ProcessInput();
		}
	}
	MouseInput::s_OffsetX = 0.0f;
	MouseInput::s_OffsetY = 0.0f;
}

void Application::Update()
{
	if(s_DroppedModelPath != "" && s_DroppedModelPath != m_ModelPath)
	{
		m_ModelPath = s_DroppedModelPath;
		LoadModel();
		m_TexturePath = "";
		
	}
	else if (flipUVs != areUVsFlipped)
	{
		LoadModel();
		areUVsFlipped = flipUVs;
		if (m_TexturePath != "")
		{
			m_Model->AddTexture(m_TexturePath.string(), "texture_diffuse");
			m_Model->AddTexture(m_NormalTexturePath.string(), "texture_normal");
			m_Model->AddTexture(m_HeightTexturePath.string(), "texture_height");
		}
	}
	if(s_DroppedTexturePath != "" && s_DroppedTexturePath != m_TexturePath)
	{
		m_TexturePath = s_DroppedTexturePath;
		m_Model->AddTexture(m_TexturePath.string(), "texture_diffuse");
	}
	m_MainCamera.UpdateOrbitalPositionAndRotation();

	m_LightViewCamera.SetPosition(m_LightPos.x, m_LightPos.y, m_LightPos.z);
	m_LightViewCamera.LookAt(0.0f, 0.0f, 0.0f);
	m_LightSourceTrans.SetPosition(m_LightPos.x, m_LightPos.y, m_LightPos.z);
}

void Application::LoadModel()
{
	m_Model.reset();
	m_Model = std::make_unique<Model>(m_ModelPath.string(), flipUVs);
	m_MainCamera.FocusOn(*m_Model, m_ModelTrans);
	m_LightPosLimit = m_Model->GetLargestDiagonal().Length() * 10.0f;
	normalMagnitude = m_Model->GetLargestDiagonal().Length() / 100.0f;
	auto boundingBox = m_Model->GetBoundingBox();
	auto largestDiagonal = boundingBox.max - boundingBox.min;
	float modelSize = largestDiagonal.Length();
	m_QuadTrans.SetScale(10.0f * modelSize);
	m_QuadTrans.SetPosition(0.0f, boundingBox.min.y, 0.0f);
}

void Application::Render()
{
	rescale_framebuffer(m_ModelViewWidth, m_ModelViewHeight);
	DrawScene();
	DrawImGui();
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
