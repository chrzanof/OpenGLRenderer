#include "Camera.h"

void Camera::SetPosition(float x, float y, float z)
{
	this->m_position = Vector3f{ x, y, z };
}

void Camera::SetTarget(float x, float y, float z)
{
	this->m_target = Vector3f{ x, y, z };
}

void Camera::SetFov(float fov)
{
	this->m_fov = fov;
}

void Camera::SetNear(float near)
{
	this->m_near = near;
}

void Camera::SetFar(float far)
{
	this->m_far = far;
}

void Camera::SetWidth(float width)
{
	this->m_width = width;
}

void Camera::SetHeight(float height)
{
	this->m_height = height;
}

Matrix4x4_f Camera::GetViewMatrix()
{
	Vector3f direction = (m_target - m_position).Normalized();

	Vector3f right = Vector3f::Cross(this->m_up, direction).Normalized();
	m_up = Vector3f::Cross(direction, right).Normalized();

	Matrix4x4_f cameraRotation = Matrix4x4_f{
		right.x, right.y, right.z,  0,
		m_up.x, m_up.y, m_up.z,  0,
		direction.x, direction.y, direction.z,  0,
		0,   0,   0,    1
	};

	Matrix4x4_f cameraTranslation = Matrix4x4_f{
		1, 0, 0,  -m_position.x,
		0, 1, 0,  -m_position.y,
		0, 0, 1,  -m_position.z,
		0, 0, 0,  1
	};
	return cameraRotation * cameraTranslation;
}

Matrix4x4_f Camera::GetProjectionMatrix() const
{
	return Matrix4x4_f::Perspective(m_fov, m_near, m_far, m_width, m_height);
}
