#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#define _USE_MATH_DEFINES
#include "stb_image.h"
#include <iostream>
#include <vector>

#include "Camera.h"
#include "Mesh.h"
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

	// 24 vertices: 6 faces × 4 verts each
	std::vector<Vertex> vertices = {
		// Front face (z = +0.5, normal = +Z)
		{-0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f}, // bottom-left
		{ 0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f}, // bottom-right
		{ 0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f}, // top-right
		{-0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f}, // top-left

		// Back face (z = -0.5, normal = -Z)
		{ 0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  0.0f, 0.0f}, // bottom-left
		{-0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  1.0f, 0.0f}, // bottom-right
		{-0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  1.0f, 1.0f}, // top-right
		{ 0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  0.0f, 1.0f}, // top-left

		// Left face (x = -0.5, normal = -X)
		{-0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f}, // bottom-left
		{-0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f}, // bottom-right
		{-0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f}, // top-right
		{-0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f}, // top-left

		// Right face (x = +0.5, normal = +X)
		{ 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f}, // bottom-left
		{ 0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f}, // bottom-right
		{ 0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f}, // top-right
		{ 0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f}, // top-left

		// Top face (y = +0.5, normal = +Y)
		{-0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f}, // bottom-left
		{ 0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f}, // bottom-right
		{ 0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f}, // top-right
		{-0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f}, // top-left

		// Bottom face (y = -0.5, normal = -Y)
		{-0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f}, // bottom-left
		{ 0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f}, // bottom-right
		{ 0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 1.0f}, // top-right
		{-0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 1.0f}  // top-left
	};

	// 36 indices (2 triangles per face × 6 faces)
	std::vector<unsigned int> elements = {
		0,  1,  2,   2,  3,  0,       // Front
		4,  5,  6,   6,  7,  4,       // Back
		8,  9, 10,  10, 11,  8,       // Left
		12, 13, 14, 14, 15, 12,       // Right
		16, 17, 18, 18, 19, 16,       // Top
		20, 21, 22, 22, 23, 20        // Bottom
	};


	ShaderProgram program = ShaderProgram(
		"shaders/vert.glsl",
		"shaders/frag.glsl"
	);

	Texture2d texture = Texture2d("textures/container2.png");

	GLuint transformLocation = glGetUniformLocation(program.GetId(), "transform");

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	float angle = 0.0f;

	Camera camera;
	camera.SetFov(ToRadians(90.0f));
	camera.SetNear(1.0f);
	camera.SetFar(10.0f);
	camera.SetWidth(window.GetWidth());
	camera.SetHeight(window.GetHeight());

	WorldTrans worldTrans;

	Mesh mesh{vertices, elements};
	mesh.setupMesh();

	

	// pêtla renderingu
	while (!window.ShouldClose())
	{
		window.ProcessInput();

		camera.SetTarget(0.0f, 0.0f, 0.0f);
		camera.SetPosition(4 * sin(angle), 0.0f, 2 * cos(angle));

		worldTrans.SetPosition(0.0f, 0.0f, 0.0f);
		worldTrans.SetRotation(angle, angle, 0.0f);
		worldTrans.SetScale(1.0f);

		auto finalTransform = camera.GetProjectionMatrix() * camera.GetViewMatrix() * worldTrans.GetMatrix();

		glClear(GL_COLOR_BUFFER_BIT);

		glUniformMatrix4fv(transformLocation, 1, GL_TRUE, finalTransform.values);

		program.Bind();

		texture.Bind();

		mesh.Bind();

		glDrawElements(GL_TRIANGLES, mesh.GetIndicesData().size(), GL_UNSIGNED_INT, 0);

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
