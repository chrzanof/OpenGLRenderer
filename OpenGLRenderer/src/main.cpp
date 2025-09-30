#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#define _USE_MATH_DEFINES
#include "stb_image.h"
#include <iostream>
#include <vector>

#include "Camera.h"
#include "math/Vector2f.h"
#include "math/Vector3f.h"
#include "math/Vector4f.h"
#include "math/Matrix4x4_f.h"
#include "Vertex.h"


#include "Shader.h"
#include "ShaderProgram.h"
#include "WorldTrans.h"
#include "Texture2d.h"
#include "Window.h"

#define ToRadians(x) x * M_PI / 180.0f

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


GLuint CreateVertexBufferObject(Vertex* vertices, GLsizeiptr size, GLenum usage);

GLuint CreateIndexBufferObject(unsigned int* elements, GLsizeiptr size, GLenum usage);

GLuint CreateVertexArrayObject();

int gWidth = 1500;
int gHeight = 1000;

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

	// Vertex definition (x, y, z, r, g, b, a, u, v)
	std::vector<Vertex> vertices = {
		// Front face (z = +0.5)
		Vertex(-0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f), // 0 bottom-left
		Vertex(0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f), // 1 bottom-right
		Vertex(0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f), // 2 top-right
		Vertex(-0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f, 1.0f,   0.0f, 1.0f), // 3 top-left

		// Back face (z = -0.5)
		Vertex(-0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f, 1.0f,   1.0f, 0.0f), // 4 bottom-right
		Vertex(0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f), // 5 bottom-left
		Vertex(0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f), // 6 top-left
		Vertex(-0.5f,  0.5f, -0.5f,   0.3f, 0.3f, 0.3f, 1.0f,   1.0f, 1.0f) // 7 top-right
	};

	std::vector<unsigned int> elements = {
		// Front face
		0, 1, 2,
		2, 3, 0,

		// Right face
		1, 5, 6,
		6, 2, 1,

		// Back face
		5, 4, 7,
		7, 6, 5,

		// Left face
		4, 0, 3,
		3, 7, 4,

		// Top face
		3, 2, 6,
		6, 7, 3,

		// Bottom face
		4, 5, 1,
		1, 0, 4
	};

	ShaderProgram program = ShaderProgram(
		"shaders/vert.glsl",
		"shaders/frag.glsl"
	);

	Texture2d texture = Texture2d("textures/container2.png");
	

	GLuint VAO = CreateVertexArrayObject();

	GLuint VBO = CreateVertexBufferObject(&vertices.front(), sizeof(vertices.front()) * vertices.size(), GL_STATIC_DRAW);
	GLuint IBO = CreateIndexBufferObject(&elements.front(), sizeof(elements.front()) * elements.size(), GL_STATIC_DRAW);

	GLuint gScaleLocation = glGetUniformLocation(program.GetId(), "gScale");

	GLuint transformLocation = glGetUniformLocation(program.GetId(), "transform");

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	float angle = 0.0f;

	Camera camera;
	camera.SetFov(ToRadians(90.0f));
	camera.SetNear(1.0f);
	camera.SetFar(10.0f);
	camera.SetWidth(gWidth);
	camera.SetHeight(gHeight);

	WorldTrans worldTrans;

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3f));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3f) + sizeof(Vector4f)));

	// pêtla renderingu
	while (!window.ShouldClose())
	{
		window.ProcessInput();

		camera.SetTarget(0.0f, 0.0f, 0.0f);
		camera.SetPosition(4 * sin(angle), 0.0f, 2 * cos(angle));

		worldTrans.SetPosition(0.0f, 0.0f, 0.0f);
		worldTrans.SetRotation(angle, angle, 0.0f);
		worldTrans.SetScale(1.5f);

		auto finalTransform = camera.GetProjectionMatrix() * camera.GetViewMatrix() * worldTrans.GetMatrix();

		glClear(GL_COLOR_BUFFER_BIT);

		glUniform1f(gScaleLocation, 0.5f);
		glUniformMatrix4fv(transformLocation, 1, GL_TRUE, finalTransform.values);

		program.Bind();

		texture.Bind();

		glBindVertexArray(VAO);
		// glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);


		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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
	gWidth = width;
	gHeight = height;
}

GLuint CreateVertexBufferObject(Vertex* vertices, GLsizeiptr size, GLenum usage)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
	return VBO;
}

GLuint CreateIndexBufferObject(unsigned int* elements, GLsizeiptr size, GLenum usage)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, elements, usage);
	return VBO;
}


GLuint CreateVertexArrayObject()
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	return VAO;
}
