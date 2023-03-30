#include "stdafx.h"
#include "DrawableObject.h"

namespace Draw
{
	int DrawableObject::GetId() const noexcept
	{
		return id;
	}

	const std::string& DrawableObject::GetName() const noexcept
	{
		return name;
	}

	void DrawableObject::SetName(const std::string& name_in) noexcept
	{
		name = std::move(name);
	}

	std::string DrawableObject::GetSysName() const noexcept
	{
		return std::format("{}##{}", name, std::to_string(id));
	}

	const XMFLOAT3& DrawableObject::GetPosition() const noexcept
	{
		return pos;
	}

	void DrawableObject::SetPosition(XMFLOAT3 position) noexcept
	{
		pos = position;
	}

	XMFLOAT3 DrawableObject::GetRotation() const noexcept
	{
		return {pitch, yaw, roll};
	}

	void DrawableObject::SetRotation(XMFLOAT3 rotation) noexcept
	{
		pitch = rotation.x;
		yaw = rotation.y;
		roll = rotation.z;
	}

	XMMATRIX DrawableObject::GetTransform() const noexcept
	{
		return XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			XMMatrixTranslation(pos.x, pos.y, -pos.z) *
			XMMatrixRotationRollPitchYaw(theta, phi, chi);
	}
}
