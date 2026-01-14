#pragma once
#include "Model.h"
#include "Window.h"
#include "WorldTrans.h"
#include "math/Matrix4x4_f.h"
#include "math/Vector3f.h"

class Camera
{
public:
	Camera() = default;
	void UpdateOrbitalPositionAndRotation();
	void ProcessInput();
	void SetPosition(float x, float y, float z);
	void LookAt(float x, float y, float z);
	void SetFov(float fov);
	void SetNear(float near);
	void SetFar(float far);
	void SetWidth(float width);
	void SetHeight(float height);
	void FocusOn(const Model& model, const WorldTrans& worldTrans);
	Matrix4x4_f GetViewMatrix() const;
	Matrix4x4_f GetProjectionMatrix(float ratio) const;


private:
	Vector3f m_position = Vector3f{ 0.0f, 0.0f, 0.0f };
	Vector3f m_direction = Vector3f{ 0.0f, 0.0f, 1.0f };
	Vector3f m_up = Vector3f{0.0f, 1.0f, 0.0f};
	Vector3f m_right = Vector3f{ 1.0f, 0.0f, 0.0f };
	Vector3f m_pivotPosition = Vector3f{ 0.0f, 0.0f, 0.0f };
	float m_fov = 0.0f;
	float m_near = 0.0f;
	float m_far = 0.0f;

	float fi = 0.0f;
	float theta = 0.0f;
	float lastFi = 0.0f;
	float lastTheta = 0.0f;
	float cameraSpeed = 0.2f;

	static float r;
	static float zoomSpeed;

	Vector2f m_LastCursorPosition{ 0.0f, 0.0f };
	Vector2f m_CurrentCursorPosition{ 0.0f, 0.0f };

};

