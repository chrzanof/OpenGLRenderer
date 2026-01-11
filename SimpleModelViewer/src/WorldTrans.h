#pragma once
#include "math/Matrix4x4_f.h"
#include "math/Vector3f.h"

class WorldTrans
{
public:
	WorldTrans() = default;

	void SetScale(float scale);
	void SetScale(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetPosition(float x, float y, float z);
	const Vector3f& GetPosition() const;
	Matrix4x4_f GetMatrix() const;

private:
	Vector3f m_scale    = Vector3f(1.0f, 1.0f, 1.0f);
	Vector3f m_rotation = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f m_position = Vector3f(0.0f, 0.0f, 0.0f);

};

