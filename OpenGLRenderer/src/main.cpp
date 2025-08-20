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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

std::string ReadFile(const std::string filePath);

GLuint CreateVertexBufferObject(GLenum target, Vertex* vertices, GLsizeiptr size, GLenum usage);

GLuint CreateVertexArrayObject();

GLuint CreateShader(GLenum type, std::string source);

GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);



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
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "OpenGLRenderer", NULL, NULL);

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
	glViewport(0, 0, 1920, 1080);

	// ustawiamy callback zmiany rozmiaru okna
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Vertex vertices[] = {
		Vertex(-1.0f, -1.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(	1.0f, -1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f),
		Vertex(	0.0f,  1.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f),
	};

	std::string vsSource = R"(
	#version 330 core

	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec4 aColor;

	uniform float gScale;
	uniform mat4 translation;

	out vec4 Color;

	void main()
	{
		gl_Position = translation * vec4(aPos.x * gScale, aPos.y * gScale, aPos.z * gScale, 1.0f);
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

	GLuint VBO = CreateVertexBufferObject(GL_ARRAY_BUFFER, vertices, sizeof(vertices), GL_STATIC_DRAW);

	GLuint gScaleLocation = glGetUniformLocation(program, "gScale");

	GLuint translationLocation = glGetUniformLocation(program, "translation");

	float translation[] = {
		1.0f, 0.0f, 0.0f, 0.2f,
		0.0f, 1.0f, 0.0f, 0.2f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	


	// pêtla renderingu
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

		glUniform1f(gScaleLocation, 0.5f);
		glUniformMatrix4fv(translationLocation, 1, GL_TRUE, translation);

		glUseProgram(program);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3f));

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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

GLuint CreateVertexBufferObject(GLenum target, Vertex* vertices, GLsizeiptr size, GLenum usage)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(target, VBO);
	glBufferData(target, size, vertices, usage);
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
