#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#define _USE_MATH_DEFINES
#include "stb_image.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "Vector3f.h"
#include "Vector4f.h"
#include "Vertex.h"
#include "Matrix4x4_f.h"
#include <math.h>

#define ToRadians(x) x * M_PI / 180.0f

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

std::string ReadFile(const std::string filePath);

GLuint CreateVertexBufferObject(Vertex* vertices, GLsizeiptr size, GLenum usage);

GLuint CreateIndexBufferObject(unsigned int* elements, GLsizeiptr size, GLenum usage);

GLuint CreateVertexArrayObject();

GLuint CreateShader(GLenum type, std::string source);

GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);

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
		Vertex(-0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f, 1.0f), // 0
		Vertex(0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f, 1.0f), // 1
		Vertex(0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f), // 2
		Vertex(-0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f, 1.0f), // 3

		// Back face (z = -0.5)
		Vertex(-0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f, 1.0f), // 4
		Vertex(0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f, 1.0f), // 5
		Vertex(0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 1.0f), // 6
		Vertex(-0.5f,  0.5f, -0.5f,   0.3f, 0.3f, 0.3f, 1.0f)  // 7
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

	std::string vsSource = R"(
	#version 330 core

	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec4 aColor;

	uniform float gScale;
	uniform mat4 transform;

	out vec4 Color;

	void main()
	{
		gl_Position = transform * vec4(aPos.x * gScale, aPos.y * gScale, aPos.z * gScale, 1.0f);
		Color = aColor;
	} 
	)";
	GLuint VS = CreateShader(GL_VERTEX_SHADER, vsSource);

	std::string fsSource = R"(
	#version 330 core

	in vec4 Color;

	out vec4 FragColor;
	
	void main()
	{
		FragColor = Color;  
	}
	)";
	GLuint FS = CreateShader(GL_FRAGMENT_SHADER, fsSource);


	GLuint program = CreateProgram(VS, FS);
	

	glDeleteShader(VS);
	glDeleteShader(FS);

	GLuint VAO = CreateVertexArrayObject();

	GLuint VBO = CreateVertexBufferObject(vertices, sizeof(vertices), GL_STATIC_DRAW);
	GLuint IBO = CreateIndexBufferObject(elements, sizeof(elements), GL_STATIC_DRAW);

	GLuint gScaleLocation = glGetUniformLocation(program, "gScale");

	GLuint transformLocation = glGetUniformLocation(program, "transform");

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	Vector3f R = Vector3f(1.0f, 0.0f, 0.0f);
	Vector3f U = Vector3f(0.0f, 1.0f, 0.0f);
	Vector3f D = Vector3f(0.0f, 0.0f, 1.0f);
	Vector3f cameraPosition = Vector3f{ 0.0f, 0.0f, -3.0f };

	Matrix4x4_f camera = Matrix4x4_f{
		R.x, R.y, R.z, -cameraPosition.x,
		U.x, U.y, U.z, -cameraPosition.y,
		D.x, D.y, D.z, -cameraPosition.z,
		0,   0,   0,    1
	};

	float angle = 0.0f;

	float fov = 90.0f;
	float n = 1.0f;
	float f = 10.0f;

	// pêtla renderingu
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		Vector3f cameraTarget = Vector3f{ 0.0f, 0.0f, 0.0f };
		Vector3f cameraPosition = Vector3f{ 4 * sin(angle), 0.0f,2 * cos(angle) };

		Vector3f D = (cameraTarget - cameraPosition).Normalized();
		
		Vector3f R = Vector3f::Cross(Vector3f(0.0f, 1.0f, 0.0f), D).Normalized();
		Vector3f U = Vector3f::Cross(D, R).Normalized();
		
		// Vector3f R = Vector3f{1.0f, 0.0f, 0.0f};
		// Vector3f U = Vector3f{0.0f, 1.0f, 0.0f};
		// Vector3f D = Vector3f{0.0f, 0.0f, 1.0f};

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

		auto translation = Matrix4x4_f::Translation(Vector3f{ 0.0f, 0.0f, 0.0f });

		auto rotation = Matrix4x4_f::RotationXYZ(Vector3f{angle, angle, 0});

		auto scale = Matrix4x4_f::Scale(Vector3f{ 1.5f, 1.5f, 1.5f });

		auto world = translation * rotation * scale;

		auto view = cameraRotation * cameraTranslation;

		auto finalTransform = projection * view * world;

		glClear(GL_COLOR_BUFFER_BIT);

		glUniform1f(gScaleLocation, 0.5f);
		glUniformMatrix4fv(transformLocation, 1, GL_TRUE, finalTransform.values);

		glUseProgram(program);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3f));

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

std::string ReadFile(const std::string filePath)
{
	std::ifstream file;
	file.open(filePath);
	if(file.fail())
	{
		std::cout << "Error opening the file: " << filePath << "\n";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
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

GLuint CreateShader(GLenum type, std::string source)
{
	GLuint shader = glCreateShader(type);
	const char* sourceCStr = source.c_str();
	glShaderSource(shader, 1, &sourceCStr, NULL);
	glCompileShader(shader);

	GLint shaderCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);
	if (shaderCompiled != GL_TRUE)
	{
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(shader, 1024, &log_length, message);
		std::cout << message;
	}
	return shader;
}
GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glUseProgram(program);
	return program;
}
