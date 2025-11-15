#include "Application.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	WindowSpecs window_specs{ 1500, 1000, "OBJ Model Viewer" };

	ApplicationSpecs appSpecs {
		window_specs,
		"shaders/vert.glsl",
		"shaders/frag.glsl"
	};

	Application application(appSpecs);
	application.Run();

	return 0;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

