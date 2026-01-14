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

	static Vector3f Cross(Vector3f v1, Vector3f v2)
	{
		return Vector3f{
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
		};
	}

	Vector3f Normalized()
	{
		float length = sqrt(x * x + y * y + z * z);
		return Vector3f{ x / length, y / length, z / length };
	}

	float Length()
	{
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	Vector3f operator+(Vector3f other)
	{
		return Vector3f(x + other.x, y + other.y, z + other.z);
	}
	Vector3f operator-(Vector3f other)
	{
		return Vector3f(x - other.x, y - other.y, z - other.z);
	}
	Vector3f operator*(float value)
	{
		return Vector3f(x * value, y * value, z * value);
	}
};
