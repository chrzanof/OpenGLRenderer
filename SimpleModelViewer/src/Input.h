#pragma once
#include "math/Vector2f.h"

struct MouseInput
{
	static float offsetX;
	static float offsetY;

	static bool leftButtonClicked;
	static bool rightButtonClicked;

	static Vector2f position;
	static Vector2f lastPosition;
};