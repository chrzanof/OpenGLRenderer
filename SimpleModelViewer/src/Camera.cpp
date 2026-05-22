#include "Camera.h"

#include <corecrt_math_defines.h>

#include "Application.h"

#include "Input.h"

void Camera::SetPosition(float x, float y, float z)
{
	this->m_Position = Vector3f{ x, y, z };
}

void Camera::LookAt(float x, float y, float z)
{
	m_Direction = (Vector3f{ x, y, z } - m_Position).Normalized();
	m_Right = Vector3f::Cross(Vector3f{ 0.0f, 1.0f, 0.0f }, m_Direction).Normalized();
	m_Up = Vector3f::Cross(m_Direction, m_Right).Normalized();
}

void Camera::SetFov(float fov)
{
	this->m_Fov = fov;
}

void Camera::SetNearPlane(float nearPlane)
{
	this->m_NearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	this->m_FarPlane = farPlane;
}

Matrix4x4_f Camera::GetViewMatrix() const
{

	Matrix4x4_f cameraRotation = Matrix4x4_f{
		m_Right.x, m_Right.y, m_Right.z,  0,
		m_Up.x, m_Up.y, m_Up.z,  0,
		m_Direction.x, m_Direction.y, m_Direction.z,  0,
		0,   0,   0,    1
	};

	Matrix4x4_f cameraTranslation = Matrix4x4_f{
		1, 0, 0,  -m_Position.x,
		0, 1, 0,  -m_Position.y,
		0, 0, 1,  -m_Position.z,
		0, 0, 0,  1
	};
	return cameraRotation * cameraTranslation;
}

Matrix4x4_f Camera::GetProjectionMatrix(float aspectRatio) const
{
	return Matrix4x4_f::Perspective(m_Fov, m_NearPlane, m_FarPlane, aspectRatio);
}

void OrbitalCamera::UpdateOrbitalPositionAndRotation()
{
	auto cameraPitch = Matrix4x4_f::RotationX(TO_RADIANS(m_Azimuth));
	auto cameraYaw = Matrix4x4_f::RotationY(TO_RADIANS(m_Elevation));

	Vector4f cameraVector{ 0.0f, 0.0f, -m_Radius, 1.0f };
	cameraVector = cameraYaw * cameraPitch * cameraVector;

	cameraVector = cameraVector + Vector4f{ m_PivotPosition, 0.0f };

	SetPosition(cameraVector.x, cameraVector.y, cameraVector.z);
	LookAt(m_PivotPosition.x, m_PivotPosition.y, m_PivotPosition.z);
}

void OrbitalCamera::ProcessInput()
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
		return;

	m_LastAzimuth = m_Azimuth;
	m_LastElevation = m_Elevation;
	Vector2f deltaCursorPosition = MouseInput::s_Position - MouseInput::s_LastPosition;

	if(MouseInput::s_LeftButtonClicked)
	{
		m_Azimuth = m_LastAzimuth + deltaCursorPosition.y * m_MovementSpeed;
		m_Elevation = m_LastElevation + deltaCursorPosition.x * m_MovementSpeed;

		if (m_Azimuth > 89.5f)
			m_Azimuth = 89.5f;
		else if (m_Azimuth < -89.5f)
			m_Azimuth = -89.5f;
	}
	else if(MouseInput::s_RightButtonClicked)
	{
		m_PivotPosition = m_PivotPosition + m_Right * -deltaCursorPosition.x * m_ZoomSpeed *  0.01f;
		m_PivotPosition = m_PivotPosition + m_Up * deltaCursorPosition.y * m_ZoomSpeed * 0.01f;
	}

	m_Radius -= MouseInput::s_OffsetY * m_ZoomSpeed;
	if (m_Radius < m_NearPlane)
		m_Radius = m_NearPlane;
}


void OrbitalCamera::FocusOn(const Model& model, const WorldTrans& worldTrans)
{
	// calculate camera distance and speed
	float cameraDistanceModifier = 2.0f;
	float l = model.GetLargestDiagonal().Length();
	m_Radius = (l * 0.5f) * tan(m_Fov * 0.5f) * cameraDistanceModifier;
	m_ZoomSpeed = m_Radius * 0.125;
	m_PivotPosition = worldTrans.GetPosition();
}
