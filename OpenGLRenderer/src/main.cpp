#include "Application.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	ApplicationSpecs application_specs {
		{1500, 1000, "OBJ Model Viewer"},
		"models/bear-head/source/bear.fbx"
	};

	Application application(application_specs);
	application.Run();

	return 0;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

