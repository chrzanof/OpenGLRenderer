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
    std::unique_ptr<ShaderProgram> m_SimpleDepthShader;
    std::unique_ptr<ShaderProgram> m_DepthMapPreviewShader;
    std::unique_ptr<ShaderProgram> m_LightSourceShader;
    std::unique_ptr<ShaderProgram> m_ShowNormalsShader;
    std::unique_ptr<ShaderProgram> m_WireframeShader;
    bool showModel = true;
    bool showNormals = false;
    bool showWireframe = false;
    std::unique_ptr<Model> m_Model;
    std::unique_ptr<Quad> m_Quad;
    std::unique_ptr<Skybox> m_Skybox;
    WorldTrans m_ModelTrans;
    WorldTrans m_QuadTrans;
    WorldTrans m_LightSourceTrans;
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
    // debug depth map
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    void renderQuad()
    {
        if (quadVAO == 0)
        {
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }
};

