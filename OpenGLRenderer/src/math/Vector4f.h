#pragma once

class Vector3f
{
public:
	float x;
	float y;
	float z;
	float w;

	Vector3f(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
	{
	}

	Vector3f(Vector3f vector3f, float _w): x(vector3f.x), y(vector3f.y), z(vector3f.z), w(_w)
	{
	}

	static float Dot(Vector3f v1, Vector3f v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	Vector3f Normalized()
	{
		float length = sqrt(x * x + y * y + z * z + w * w);
		return Vector3f{ x / length, y / length, z / length, w / length };
	}

	Vector3f operator+(Vector3f other)
	{
		return Vector3f(x + other.x, y + other.y, z + other.z, w + other.w);
	}
	Vector3f operator-(Vector3f other)
	{
		return Vector3f(x - other.x, y - other.y, z - other.z, w - other.w);
	}

};
