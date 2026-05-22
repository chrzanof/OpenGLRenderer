#pragma once

#include "Window.h"
#include "Camera.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "WorldTrans.h"
#include <string>
#include "imgui/filedialog/ImGuiFileDialog.h"
#include <filesystem>
#include "Quad.h"
#include "TextureCubeMap.h"
#include "Skybox.h"

#define TO_RADIANS(x) ((x) * M_PI / 180.0f)


struct ApplicationSpecs
{
    WindowSpecs windowSpecs;
    std::string vertexShaderPath;
    std::string fragmentShaderPath;
    std::string defaultModelPath;
    std::string defaultTexturePath;
    std::vector<std::string> skyboxFaces;
};

class Application
{
public:
    Application(const ApplicationSpecs& appSpecs);
    ~Application();
    void Run();

private:
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void DropCallback(GLFWwindow* window, int count, const char** paths);
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

    void InitImGui(GLFWwindow* window);
    void ShutdownImGui();
    void DrawImGui();
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();

private:
    Window m_Window;
    OrbitalCamera m_MainCamera;
    Camera m_LightViewCamera;
    std::unique_ptr<ShaderProgram> m_ModelShader;
    std::unique_ptr<ShaderProgram> m_SkyboxShader;
    std::unique_ptr<Model> m_Model;
    std::unique_ptr<Skybox> m_Skybox;
    WorldTrans m_WorldTrans;
    Vector3f m_LightPos;
    Vector3f m_LightColor;
    float m_LightPosLimit = 10.0f;

    std::filesystem::path m_TexturePath = "";
    std::filesystem::path m_ModelPath = "";

    const unsigned int SHADOW_MAP_WIDTH = 1024, SHADOW_MAP_HEIGHT = 1024;
    unsigned int m_DepthMapFBO = 0;
    unsigned int m_DepthMap = 0;

    static inline std::filesystem::path s_DroppedModelPath = "";
    static inline std::filesystem::path s_DroppedTexturePath = "";

    static inline std::vector<std::string> s_AssimpExtensions = {
    ".3ds", ".ase", ".ac", ".ac3d", ".blend", ".b3d", ".bvh", ".c4d", ".cob",
    ".dae", ".dxf", ".fbx", ".glb", ".gltf", ".hkt", ".hkx", ".ifc", ".ifcxml",
    ".irrmesh", ".lwo", ".lws", ".md2", ".md3", ".md5anim", ".md5mesh", ".ms3d",
    ".obj", ".off", ".ply", ".stl", ".x", ".xgl", ".wrl", ".wrz", ".3mf",
    ".assbin", ".x3d", ".mesh", ".xml"
    };
	static inline std::vector<std::string> s_TextureExtensions = {
    ".jpg", ".jpeg", ".png", ".bmp", ".tga", ".psd", ".gif", ".hdr", ".pic", ".pnm"
    };
};

