#pragma once
#include "Mesh.h"

class Quad : public Mesh
{
public:
	Quad(): Mesh({
			Vertex{ Vector3f{-0.5f, -0.5f, 0.0f},  Vector3f{0.0f, 0.0f, 1.0f}, Vector2f{0.0f, 0.0f} },
			Vertex{ Vector3f{ 0.5f, -0.5f, 0.0f},  Vector3f{0.0f, 0.0f, 1.0f}, Vector2f{1.0f, 0.0f} },
			Vertex{ Vector3f{ 0.5f,  0.5f, 0.0f},  Vector3f{0.0f, 0.0f, 1.0f}, Vector2f{1.0f, 1.0f} },
			Vertex{ Vector3f{-0.5f,  0.5f, 0.0f},  Vector3f{0.0f, 0.0f, 1.0f}, Vector2f{0.0f, 1.0f} },},
		{ 0, 1, 2,  2, 3, 0 }
	)
	{
	}
};
