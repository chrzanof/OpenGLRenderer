#include "Application.h"

int main()
{
	WindowSpecs windowSpecs {
		.width{ 1200 },
		.height{ 800 },
		.title{ "Model Viewer" }
	};

	ApplicationSpecs appSpecs{
		.windowSpecs{ windowSpecs },
		.vertexShaderPath{ "shaders/vert.glsl" },
		.fragmentShaderPath{ "shaders/frag.glsl" },
		.defaultModelPath{ "models/cube.obj" },
		.defaultTexturePath{ "models/brickwall.jpg" },
		.defaultNormalMapPath{"models/brickwall_normal.jpg"},
		.skyboxFaces{ 
			{
			"models/skybox/right.jpg",
			"models/skybox/left.jpg",
			"models/skybox/top.jpg",
			"models/skybox/bottom.jpg",
			"models/skybox/front.jpg",
			"models/skybox/back.jpg"
			} 
		}
	};

	Application application(appSpecs);
	application.Run();

	return 0;
}

