#include "Input.h"

float MouseInput::offsetX = 0.0f;
float MouseInput::offsetY = 0.0f;

bool MouseInput::leftButtonClicked = false;
bool MouseInput::rightButtonClicked = false;

Vector2f MouseInput::position = { 0.0f, 0.0f };
Vector2f MouseInput::lastPosition = { 0.0f, 0.0f };