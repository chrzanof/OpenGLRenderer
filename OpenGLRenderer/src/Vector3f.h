#pragma once

class Vector3f
{
public:
	float x;
	float y;
	float z;

	Vector3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{
	}

	static float Dot(Vector3f v1, Vector3f v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	Vector3f operator+(Vector3f other)
	{
		return Vector3f(x + other.x, y + other.y, z + other.z);
	}
	Vector3f operator-(Vector3f other)
	{
		return Vector3f(x - other.x, y - other.y, z - other.z);
	}
};
