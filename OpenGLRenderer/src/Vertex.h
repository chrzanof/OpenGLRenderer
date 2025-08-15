#pragma once
#include <windows.h>

#include "Vector3f.h"
#include "Vector4f.h"

class Vertex
{
public:
	Vector3f position;
	Vector4f color;

	Vertex(Vector3f _position, Vector4f _color) : position(_position), color(_color) {}

	Vertex(float _x, float _y, float _z, float _r, float _g, float _b, float _a): position(Vector3f(_x, _y, _z)), color(Vector4f(_r, _g, _b, _a))
	{
	}
};
