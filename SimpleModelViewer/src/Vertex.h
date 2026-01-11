#pragma once

#include "math/Vector3f.h"
#include "math/Vector3f.h"
#include "math/Vector2f.h"

class Vertex
{
public:
	Vector3f position;
	Vector3f normal;
	Vector2f uv;

	Vertex(Vector3f position, Vector3f normal, Vector2f texCoord)
	:
	position(position),
	normal(normal),
	uv(texCoord) {}

	Vertex(float x, float y, float z,
	       float nx, float ny, float nz,
		   float u, float v)
	:
	position(Vector3f(x, y, z)),
	normal(Vector3f(nx, ny, nz)),
	uv(Vector2f(u, v)) {}
};
