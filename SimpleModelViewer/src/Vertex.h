#pragma once

#include "math/Vector3f.h"
#include "math/Vector2f.h"

struct Vertex
{
public:
	Vector3f position;
	Vector3f normal;
	Vector3f tangent;
	Vector2f uv;

	Vertex(Vector3f position, Vector3f normal, Vector3f tangent, Vector2f uv)
	:
	position(position),
	normal(normal),
	tangent(tangent),
	uv(uv) {}

	Vertex(float x, float y, float z,
	       float nx, float ny, float nz,
		   float tx, float ty, float tz,
		   float u, float v)
	:
	position(Vector3f(x, y, z)),
	normal(Vector3f(nx, ny, nz)),
	tangent(Vector3f(tx, ty, tz)),
	uv(Vector2f(u, v)) {}
};
