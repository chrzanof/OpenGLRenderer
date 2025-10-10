#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#define _USE_MATH_DEFINES
#include "stb_image.h"
#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "math/Vector2f.h"
#include "math/Vector3f.h"
#include "math/Vector3f.h"
#include "math/Matrix4x4_f.h"
#include "Vertex.h"


#include "Shader.h"
#include "ShaderProgram.h"
#include "WorldTrans.h"
#include "Texture2d.h"
#include "Window.h"

#define ToRadians(x) x * M_PI / 180.0f

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	WindowSpecs windowSpecs{ 1500, 1000, "OBJ Model Viewer" };
	Window window(windowSpecs);

	// inicjujemy glad'a
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//ustawiamy przestrzeñ do rysowania
	window.SetViewport(0, 0, window.GetWidth(), window.GetHeight());

	// ustawiamy callback zmiany rozmiaru okna
	window.SetFrameBufferSizeCallback(framebuffer_size_callback);


	ShaderProgram program = ShaderProgram(
		"shaders/vert.glsl",
		"shaders/frag.glsl"
	);

	Texture2d texture = Texture2d("models/backpack/diffuse.jpg");

	GLuint transformLocation = glGetUniformLocation(program.GetId(), "transform");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	float angle = 0.0f;

	Camera camera;
	camera.SetFov(ToRadians(90.0f));
	camera.SetNear(0.1f);
	camera.SetFar(1.0f);
	camera.SetWidth(window.GetWidth());
	camera.SetHeight(window.GetHeight());

	WorldTrans worldTrans;

	Model model{"models/backpack/backpack.obj"};


	// pêtla renderingu
	while (!window.ShouldClose())
	{
		window.ProcessInput();

		camera.SetTarget(0.0f, 0.0f, 0.0f);
		camera.SetPosition(8 * sin(angle/2), 0.0f, 8 * cos(angle/2));

		worldTrans.SetPosition(0.0f, 0.0f, 0.0f);
		worldTrans.SetRotation(angle, angle, 0.0f);
		worldTrans.SetScale(1.0f);

		auto finalTransform = camera.GetProjectionMatrix() * camera.GetViewMatrix() * worldTrans.GetMatrix();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH);

		program.Bind();
		glUniformMatrix4fv(transformLocation, 1, GL_TRUE, finalTransform.values);

		
		model.Draw(program, texture);
		

		window.SwapBuffers();

		glfwPollEvents();
		angle += 0.005f;
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
