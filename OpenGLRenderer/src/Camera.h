#pragma once
#include "math/Matrix4x4_f.h"
#include "math/Vector3f.h"

class Camera
{
public:
	Camera() = default;

	void SetPosition(float x, float y, float z);
	void SetPositionFromSphericalCoords(float theta, float fi, float r, Vector3f center);
	void SetTarget(float x, float y, float z);
	void SetFov(float fov);
	void SetNear(float near);
	void SetFar(float far);
	void SetWidth(float width);
	void SetHeight(float height);
	Matrix4x4_f GetViewMatrix();
	Matrix4x4_f GetProjectionMatrix() const;


private:
	Vector3f m_position = Vector3f{ 0.0f, 0.0f, 0.0f };
	Vector3f m_target = Vector3f{ 0.0f, 0.0f, 1.0f };
	Vector3f m_up = Vector3f{0.0f, 1.0f, 0.0f};
	float m_fov = 0.0f;
	float m_near = 0.0f;
	float m_far = 0.0f;
	float m_width = 0.0f;
	float m_height = 0.0f;
};

