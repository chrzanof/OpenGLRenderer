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

//#define _USE_MATH_DEFINES
#define ToRadians(x) x * M_PI / 180.0f


struct ApplicationSpecs
{
    WindowSpecs windowSpecs;
    std::string modelPath;
    std::string texturePath;
    std::string vertexShaderPath;
    std::string fragmentShaderPath;
};

class Application
{
private:
    Window m_Window;
    Camera m_Camera;
    std::unique_ptr<ShaderProgram> m_Program;
    std::unique_ptr<Texture2d> m_texture2d;
    WorldTrans m_WorldTrans;
    std::unique_ptr<Model> m_Model;
    Vector3f m_LightPos;

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        MouseInput::offsetX = static_cast<float>(xoffset);
        MouseInput::offsetY = static_cast<float>(yoffset);
    }

    void InitImGui(GLFWwindow* window);
    void ShutdownImGui();
    void DrawImGui();
    void ReloadModelAndTexture(const std::string& modelPath, const std::string& texturePath);

public:
    Application(ApplicationSpecs appSpecs);
    ~Application();
    void Run();
    void Setup();
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();
};

