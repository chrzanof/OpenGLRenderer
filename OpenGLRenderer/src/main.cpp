#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGLRenderer", NULL, NULL);

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
	glViewport(0, 0, 800, 600);

	// ustawiamy callback zmiany rozmiaru okna
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	Shader shader("src/shader.vs", "src/shader.fs");

	
	std::vector<float> vertices = {
		// positions          // colors           
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   
	};
	std::vector<unsigned int> indices = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// generujemy obiekty
	unsigned int vertexBufferObject;//, elementBufferObject;
	VAO vao{};
	// VBO vbo{};
	EBO ebo{};
	
	glGenBuffers(1, &vertexBufferObject);

	// podpinamy Vertex array
	vao.Bind();
	// podpinamy vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
	ebo.Bind();
	ebo.SetData(indices, 6);



	vao.SetAttributePointer(0, 3, GL_FLOAT, 6, 0);
	vao.SetAttributePointer(1, 3, GL_FLOAT, 6, 3);
	
	// pêtla renderingu
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		// glBindVertexArray(vertexArrayObject);
		vao.Bind();
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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
