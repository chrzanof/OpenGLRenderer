#include "Application.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	WindowSpecs window_specs{ 1500, 1000, "OBJ Model Viewer" };

	ApplicationSpecs appSpecsBear {
		window_specs,
		"models/bear-head/source/bear.fbx",
		"models/bear-head/textures/PM3D_Sphere3D_1DiffuseMap.tga.png",
		"shaders/vert.glsl",
		"shaders/frag.glsl"
	};

	ApplicationSpecs appSpecsBackpack{
		window_specs,
		"models/backpack/backpack.obj",
		"models/backpack/diffuse.jpg",
		"shaders/vert.glsl",
		"shaders/frag.glsl"
	};

	ApplicationSpecs appSpecsCube{
		window_specs,
		"models/cube/cube.obj",
		"textures/container2.png",
		"shaders/vert.glsl",
		"shaders/frag.glsl"
	};

	ApplicationSpecs appSpecsStalker{
		window_specs,
		"models/stalker-starcraft_2/scene.gltf",
		"models/stalker-starcraft_2/textures/Material.001_baseColor.png",
		"shaders/vert.glsl",
		"shaders/frag.glsl"
	};

	ApplicationSpecs appSpecsTeapot{
		window_specs,
		"models/teapot/teapot.obj",
		"textures/container.jpg",
		"shaders/vert.glsl",
		"shaders/frag.glsl"
	};

	Application application(appSpecsStalker);
	application.Run();

	return 0;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

