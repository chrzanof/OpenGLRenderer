#pragma once

#include "Window.h"
#include "Camera.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Texture2d.h"
#include "WorldTrans.h"

#include <iostream>
#include <string>

//#define _USE_MATH_DEFINES
#define ToRadians(x) x * M_PI / 180.0f


struct ApplicationSpecs
{
    WindowSpecs windowSpecs;
    std::string modelPath;
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

    float fi = 0.0f;
    float theta = 0.0f;
    static float r;

    Vector2f m_MouseClickedCursorPosition{0.0f, 0.0f};
    Vector2f m_CurrentCursorPosition{ 0.0f, 0.0f };
    Vector2f m_LastDeltaCursorPosition{ 0.0f, 0.0f };
    Vector2f m_DeltaCursorPosition{ 0.0f, 0.0f };
    bool m_MouseClicked = false;

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        r -= yoffset * 0.5f;
    }

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

