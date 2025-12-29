#pragma once

#include "Window.h"
#include "Camera.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Texture2d.h"
#include "WorldTrans.h"

#include <iostream>
#include <string>

#include "Input.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/filedialog/ImGuiFileDialog.h"
#include <filesystem>

#include "Quad.h"

//#define _USE_MATH_DEFINES
#define ToRadians(x) x * M_PI / 180.0f


struct ApplicationSpecs
{
    WindowSpecs windowSpecs;
    std::string vertexShaderPath;
    std::string fragmentShaderPath;
};

class Application
{
private:
    Window m_Window;
    Camera m_Camera;
    std::unique_ptr<ShaderProgram> m_Program;
    std::unique_ptr<Model> m_Model;
    std::unique_ptr<Texture2d> m_Texture2d;
    WorldTrans m_WorldTrans;
    Vector3f m_LightPos;

    std::filesystem::path m_TexturePathName = "";
    std::filesystem::path m_ModelPathName = "";

    static inline std::filesystem::path droppedModelPathName = "";
    static inline std::filesystem::path droppedTexturePathName = "";

    static inline std::vector<std::string> assimpExtensions = {
    ".3ds", ".ase", ".ac", ".ac3d", ".blend", ".b3d", ".bvh", ".c4d", ".cob",
    ".dae", ".dxf", ".fbx", ".glb", ".gltf", ".hkt", ".hkx", ".ifc", ".ifcxml",
    ".irrmesh", ".lwo", ".lws", ".md2", ".md3", ".md5anim", ".md5mesh", ".ms3d",
    ".obj", ".off", ".ply", ".stl", ".x", ".xgl", ".wrl", ".wrz", ".3mf",
    ".assbin", ".x3d", ".mesh", ".xml"
    };

    static inline std::vector<std::string> textureExtensions = {
    ".jpg", ".jpeg", ".png", ".bmp", ".tga", ".psd", ".gif", ".hdr", ".pic", ".pnm"
    };

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse) return;
        MouseInput::offsetX = static_cast<float>(xoffset);
        MouseInput::offsetY = static_cast<float>(yoffset);
    }

    static void drop_callback(GLFWwindow* window, int count, const char** paths)
    {
        
        for (int i = 0; i < count; i++)
        {
            std::filesystem::path droppedFilePath(paths[i]);
            std::string extension = droppedFilePath.extension().string();

            if(std::find(textureExtensions.begin(), textureExtensions.end(),extension) != textureExtensions.end())
            {
                droppedTexturePathName = droppedFilePath;
                continue;
            }

            if (std::find(assimpExtensions.begin(), assimpExtensions.end(), extension) != assimpExtensions.end())
            {
                droppedModelPathName = droppedFilePath;
            }
        }
        
    }

    void InitImGui(GLFWwindow* window);
    void ShutdownImGui();
    void DrawImGui();

public:
    Application(ApplicationSpecs appSpecs);
    ~Application();
    void Run();
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();
};

