#pragma once
#include "Model.h"
#include "WorldTrans.h"
#include "math/Matrix4x4_f.h"
#include "math/Vector3f.h"
#include "math/Vector2f.h"

class Camera 
{
public:
	Camera() = default;
	void SetPosition(float x, float y, float z);
	void LookAt(float x, float y, float z);
	void SetFov(float fov);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);
	Matrix4x4_f GetViewMatrix() const;
	Matrix4x4_f GetProjectionMatrix(float aspectRatio) const;

protected:
	Vector3f m_Position{ 0.0f, 0.0f, 0.0f };
	Vector3f m_Direction{ 0.0f, 0.0f, 1.0f };
	Vector3f m_Up{ 0.0f, 1.0f, 0.0f };
	Vector3f m_Right{ 1.0f, 0.0f, 0.0f };

	float m_Fov = 0.0f;
	float m_NearPlane = 0.0f;
	float m_FarPlane = 0.0f;
};


class OrbitalCamera : public Camera
{
public:
	OrbitalCamera() = default;
	void UpdateOrbitalPositionAndRotation();
	void ProcessInput();
	void FocusOn(const Model& model, const WorldTrans& worldTrans);
	void SetAzimuth(float value);
	void SetElevation(float value);


private:
	Vector3f m_PivotPosition{ 0.0f, 0.0f, 0.0f };

	float m_Radius = 4.0f;
	float m_Azimuth = 0.0f;
	float m_Elevation = 0.0f;
	float m_LastAzimuth = 0.0f;
	float m_LastElevation = 0.0f;

	float m_MovementSpeed = 0.2f;
	float m_ZoomSpeed = 0.5f;

	Vector2f m_LastCursorPosition{ 0.0f, 0.0f };
	Vector2f m_CurrentCursorPosition{ 0.0f, 0.0f };
};