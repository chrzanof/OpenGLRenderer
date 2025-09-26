#pragma once

#include "math/Vector3f.h"
#include "math/Vector4f.h"
#include "math/Vector2f.h"

class Vertex
{
public:
	Vector3f position;
	Vector4f color;
	Vector2f texel;

	Vertex(Vector3f _position, Vector4f _color, Vector2f _texel)
	:
	position(_position),
	color(_color),
	texel(_texel) {}

	Vertex(float _x, float _y, float _z,
	       float _r, float _g, float _b, float _a,
		   float _u, float _v)
	:
	position(Vector3f(_x, _y, _z)),
	color(Vector4f(_r, _g, _b, _a)),
	texel(Vector2f(_u, _v)) {}
};
