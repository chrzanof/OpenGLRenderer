#pragma once
#include <cmath>
class Vector2f
{
public:
    float x;
    float y;

    Vector2f(float _x, float _y) : x(_x), y(_y)
    {
    }

    static float Dot(Vector2f v1, Vector2f v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    
    static float Cross(Vector2f v1, Vector2f v2)
    {
        return v1.x * v2.y - v1.y * v2.x;
    }

    Vector2f Normalized()
    {
        float length = sqrt(x * x + y * y);
        return Vector2f{ x / length, y / length };
    }

    Vector2f operator+(Vector2f other)
    {
        return Vector2f(x + other.x, y + other.y);
    }

    Vector2f operator-(Vector2f other)
    {
        return Vector2f(x - other.x, y - other.y);
    }
};
