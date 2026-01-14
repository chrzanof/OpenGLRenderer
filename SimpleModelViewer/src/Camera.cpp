#include "Camera.h"

#include <corecrt_math_defines.h>

#include "Application.h"

#include "Input.h"

float Camera::r = 4.0f;
float Camera::zoomSpeed = 0.5f;

void Camera::SetPosition(float x, float y, float z)
{
	this->m_position = Vector3f{ x, y, z };
}

void Camera::UpdateOrbitalPositionAndRotation()
{
	auto cameraPitch = Matrix4x4_f::RotationX(ToRadians(fi));
	auto cameraYaw = Matrix4x4_f::RotationY(ToRadians(theta));

	Vector4f cameraVector{ 0.0f, 0.0f, -r, 1.0f };
	cameraVector = cameraYaw * cameraPitch * cameraVector;

	cameraVector = cameraVector + Vector4f{ m_pivotPosition, 0.0f };

	SetPosition(cameraVector.x, cameraVector.y, cameraVector.z);
	LookAt(m_pivotPosition.x, m_pivotPosition.y, m_pivotPosition.z);
}

void Camera::ProcessInput()
{
	lastFi = fi;
	lastTheta = theta;

	if(MouseInput::leftButtonClicked)
	{
		Vector2f deltaCursorPosition = MouseInput::position - MouseInput::lastPosition;
		fi = lastFi + deltaCursorPosition.y * cameraSpeed;
		theta = lastTheta + deltaCursorPosition.x * cameraSpeed;

		if (fi > 89.5f)
			fi = 89.5f;
		else if (fi < -89.5f)
			fi = -89.5f;
	}
	else if(MouseInput::rightButtonClicked)
	{
		Vector2f deltaCursorPosition = MouseInput::position - MouseInput::lastPosition;
		m_pivotPosition = m_pivotPosition + m_right * -deltaCursorPosition.x * 0.01f;
		m_pivotPosition = m_pivotPosition + m_up * deltaCursorPosition.y * 0.01f;
	}

	r -= MouseInput::offsetY * zoomSpeed;
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


void Camera::FocusOn(const Model& model, const WorldTrans& worldTrans)
{
	// calculate bounding box
	auto& meshes = model.GetMeshes();
	auto firstVertex = meshes.at(0).GetVerticesData().at(0).position;
	float minX{firstVertex.x}, minY{firstVertex.y}, minZ{firstVertex.z},
	maxX{firstVertex.x}, maxY{firstVertex.y}, maxZ{firstVertex.z};
	for(auto& mesh : meshes)
	{
		for(auto& vertex : mesh.GetVerticesData())
		{
			minX = std::min(minX, vertex.position.x);
			minY = std::min(minY, vertex.position.y);
			minZ = std::min(minZ, vertex.position.z);

			maxX = std::max(maxX, vertex.position.x);
			maxY = std::max(maxY, vertex.position.y);
			maxZ = std::max(maxZ, vertex.position.z);
		}
	}
	Vector3f largestDiagonal{ maxX - minX, maxY - minY, maxZ - minZ };

	// calculate camera distance and speed
	float cameraDistanceModifier = 2.0f;
	float l = largestDiagonal.Length();
	r = (l * 0.5f) * tan(m_fov * 0.5f) * cameraDistanceModifier;
	zoomSpeed = r * 0.125;
	UpdateOrbitalPositionAndRotation();
}

Matrix4x4_f Camera::GetViewMatrix() const
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

Matrix4x4_f Camera::GetProjectionMatrix(float ratio) const
{
	return Matrix4x4_f::Perspective(m_fov, m_near, m_far, ratio);
}
