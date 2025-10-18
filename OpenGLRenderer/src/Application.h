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

    float fi = 90.0f;
    float theta = 90.0f;
    float r = 8.0f;

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

