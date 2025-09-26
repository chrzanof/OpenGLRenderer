#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#define _USE_MATH_DEFINES
#include "stb_image.h"
#include <iostream>

#include "math/Vector2f.h"
#include "math/Vector3f.h"
#include "math/Vector4f.h"
#include "math/Matrix4x4_f.h"
#include "Vertex.h"


#include "Shader.h"
#include "ShaderProgram.h"
#include "WorldTrans.h"

#define ToRadians(x) x * M_PI / 180.0f

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

GLuint CreateVertexBufferObject(Vertex* vertices, GLsizeiptr size, GLenum usage);

GLuint CreateIndexBufferObject(unsigned int* elements, GLsizeiptr size, GLenum usage);

GLuint CreateVertexArrayObject();

GLuint CreateTexture2d(const std::string& fileName);

int gWidth = 1500;
int gHeight = 1000;

int main()
{
	// inicjujemy GLFW
	if (!glfwInit())
	{
		std::cout << "Error initializing glfw." << std::endl;
		return -1;
	}
	// configurujemy okno
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// tworzymy okno
	GLFWwindow* window = glfwCreateWindow(gWidth, gHeight, "OpenGLRenderer", NULL, NULL);

	if (!window)
	{
		std::cout << "Error creating window." << std::endl;
		glfwTerminate();
		return -1;
	}
	// ustawiamy context
	glfwMakeContextCurrent(window);
	// inicjujemy glad'a
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//ustawiamy przestrzeñ do rysowania
	glViewport(0, 0, gWidth, gHeight);

	// ustawiamy callback zmiany rozmiaru okna
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Vertex definition (x, y, z, r, g, b, a)
	Vertex vertices[] = {
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

	unsigned int elements[] = {
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

	GLuint texture = CreateTexture2d("textures/concrete.jpg");
	

	GLuint VAO = CreateVertexArrayObject();

	GLuint VBO = CreateVertexBufferObject(vertices, sizeof(vertices), GL_STATIC_DRAW);
	GLuint IBO = CreateIndexBufferObject(elements, sizeof(elements), GL_STATIC_DRAW);

	GLuint gScaleLocation = glGetUniformLocation(program.GetId(), "gScale");

	GLuint transformLocation = glGetUniformLocation(program.GetId(), "transform");

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	Vector3f R = Vector3f(1.0f, 0.0f, 0.0f);
	Vector3f U = Vector3f(0.0f, 1.0f, 0.0f);
	Vector3f D = Vector3f(0.0f, 0.0f, 1.0f);
	Vector3f cameraPosition = Vector3f{ 0.0f, 0.0f, -3.0f };

	float angle = 0.0f;

	float fov = 90.0f;
	float n = 1.0f;
	float f = 10.0f;

	WorldTrans worldTrans;

	// pêtla renderingu
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		Vector3f cameraTarget = Vector3f{ 0.0f, 0.0f, 0.0f };
		Vector3f cameraPosition = Vector3f{ 4 * sin(angle), 0.0f,2 * cos(angle) };

		Vector3f D = (cameraTarget - cameraPosition).Normalized();
		
		Vector3f R = Vector3f::Cross(Vector3f(0.0f, 1.0f, 0.0f), D).Normalized();
		Vector3f U = Vector3f::Cross(D, R).Normalized();

		Matrix4x4_f cameraRotation = Matrix4x4_f{
			R.x, R.y, R.z,  0,
			U.x, U.y, U.z,  0,
			D.x, D.y, D.z,  0,
			0,   0,   0,    1
		};

		Matrix4x4_f cameraTranslation = Matrix4x4_f{
			1, 0, 0,  -cameraPosition.x,
			0, 1, 0,  -cameraPosition.y,
			0, 0, 1,  -cameraPosition.z,
			0, 0, 0,  1
		};
		
		auto projection = Matrix4x4_f::Perspective(ToRadians(fov), n, f, gWidth, gHeight);

		worldTrans.SetPosition(0.0f, 0.0f, 0.0f);
		worldTrans.SetRotation(angle, angle, 0.0f);
		worldTrans.SetScale(1.5f);

		auto view = cameraRotation * cameraTranslation;

		auto finalTransform = projection * view * worldTrans.GetMatrix();

		glClear(GL_COLOR_BUFFER_BIT);

		glUniform1f(gScaleLocation, 0.5f);
		glUniformMatrix4fv(transformLocation, 1, GL_TRUE, finalTransform.values);

		program.Use();

		glBindTexture(GL_TEXTURE_2D, texture);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3f));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3f) + sizeof(Vector4f)));

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);


		glfwSwapBuffers(window);
		glfwPollEvents();
		angle += 0.005f;
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	gWidth = width;
	gHeight = height;
}

void processInput(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

GLuint CreateTexture2d(const std::string& fileName)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

GLuint CreateVertexBufferObject(Vertex* vertices, GLsizeiptr size, GLenum usage)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
	return VBO;
}

GLuint CreateIndexBufferObject( unsigned int* elements, GLsizeiptr size, GLenum usage)
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
