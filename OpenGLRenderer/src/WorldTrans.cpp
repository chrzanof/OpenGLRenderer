#include "WorldTrans.h"

void WorldTrans::SetScale(float scale)
{
	this->m_scale = Vector3f(scale, scale, scale);
}

void WorldTrans::SetScale(float x, float y, float z)
{
	this->m_scale = Vector3f(x, y, z);
}

void WorldTrans::SetRotation(float x, float y, float z)
{
	this->m_rotation = Vector3f(x, y, z);
}

void WorldTrans::SetPosition(float x, float y, float z)
{
	this->m_position = Vector3f(x, y, z);
}

Matrix4x4_f WorldTrans::GetMatrix() const
{
	auto translation = Matrix4x4_f::Translation(
		Vector3f{ m_position.x,m_position.y, m_position.z }
	);

	auto rotation = Matrix4x4_f::RotationXYZ(
		Vector3f{ m_rotation.x, m_rotation.y, m_rotation.z }
	);

	auto scale = Matrix4x4_f::Scale(
		Vector3f{ m_scale.x, m_scale.y, m_scale.z }
	);

	return translation * rotation * scale;
}
