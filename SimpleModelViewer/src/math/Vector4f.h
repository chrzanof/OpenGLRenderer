#pragma once

class Vector4f
{
public:
	float x;
	float y;
	float z;
	float w;

	Vector4f(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
	{
	}

	Vector4f(Vector3f vector3f, float _w): x(vector3f.x), y(vector3f.y), z(vector3f.z), w(_w)
	{
	}

	static float Dot(Vector4f v1, Vector4f v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	Vector4f Normalized()
	{
		float length = sqrt(x * x + y * y + z * z + w * w);
		return Vector4f{ x / length, y / length, z / length, w / length };
	}
	float Length()
	{
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2) + pow(w, 2));
	}

	Vector4f operator+(Vector4f other)
	{
		return Vector4f(x + other.x, y + other.y, z + other.z, w + other.w);
	}
	Vector4f operator-(Vector4f other)
	{
		return Vector4f(x - other.x, y - other.y, z - other.z, w - other.w);
	}

};
