#include "Camera.h"

void Camera::SetPosition(float x, float y, float z)
{
	this->m_position = Vector3f{ x, y, z };
}

void Camera::UpdateOrbitalPositionAndRotation(float theta, float fi, float r, Vector3f center)
{
	auto cameraPitch = Matrix4x4_f::RotationX(fi);
	auto cameraYaw = Matrix4x4_f::RotationY(theta);

	Vector4f cameraVector{ 0.0f, 0.0f, -r, 1.0f };
	cameraVector = cameraYaw * cameraPitch * cameraVector;

	cameraVector = cameraVector + Vector4f{ center, 0.0f };

	SetPosition(cameraVector.x, cameraVector.y, cameraVector.z);
	LookAt(center.x, center.y, center.z);
}

void Camera::LookAt(float x, float y, float z)
{
	m_direction = (Vector3f{ x, y, z } - m_position).Normalized();
	m_right = Vector3f::Cross(Vector3f{ 0.0f, 1.0f, 0.0f }, m_direction).Normalized();
	m_up = Vector3f::Cross(m_direction, m_right).Normalized();
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

	Matrix4x4_f cameraRotation = Matrix4x4_f{
		m_right.x, m_right.y, m_right.z,  0,
		m_up.x, m_up.y, m_up.z,  0,
		m_direction.x, m_direction.y, m_direction.z,  0,
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
