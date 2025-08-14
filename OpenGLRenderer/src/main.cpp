#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

std::string ReadFile(const std::string filePath);

GLuint CreateVertexBufferObject(GLenum target, float* vertices, GLsizeiptr size, GLenum usage);

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

	float positions[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	std::string vsSource = R"(
	#version 330 core

	layout (location = 0) in vec3 aPos;

	uniform float gScale;

	void main()
	{
		gl_Position = vec4(aPos.x * gScale, aPos.y * gScale, aPos.z * gScale, 1.0f);
	} 
	)";
	GLuint VS = CreateShader(GL_VERTEX_SHADER, vsSource);

	std::string fsSource = R"(
	#version 330 core

	out vec4 FragColor;
	
	void main()
	{
		FragColor = vec4(1.0, 0.0, 0.0, 1.0);  
	}
	)";
	GLuint FS = CreateShader(GL_FRAGMENT_SHADER, fsSource);


	GLuint program = CreateProgram(VS, FS);
	

	glDeleteShader(VS);
	glDeleteShader(FS);

	GLuint VAO = CreateVertexArrayObject();

	GLuint VBO = CreateVertexBufferObject(GL_ARRAY_BUFFER, positions, sizeof(positions), GL_STATIC_DRAW);

	GLuint gScaleLocation = glGetUniformLocation(program, "gScale");
	


	// pêtla renderingu
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

		glUniform1f(gScaleLocation, 0.5f);

		glUseProgram(program);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glEnableVertexAttribArray(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

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

GLuint CreateVertexBufferObject(GLenum target, float* vertices, GLsizeiptr size, GLenum usage)
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
