#include "stdafx.h"
#include "Camera.h"

XMMATRIX Camera::GetMatrix() const noexcept
{
	const auto pos = XMVector3Transform(
		XMVectorSet(0.0f, 0.0f, -r, 0.0f),
		XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
	);

	return XMMatrixLookAtLH(
		pos, XMVectorZero(),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	) * XMMatrixRotationRollPitchYaw(
		pitch,
		-yaw,
		roll
	);
}

void Camera::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("R", &r, 0.1f, 80.0f, "%.1f");
		ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
		ImGui::SliderAngle("Phi", &phi, -89.0f, 89.0f);
		ImGui::Text("Rotation");
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		if (ImGui::Button("Reset"))
			Reset();
	}
	ImGui::End();
}

void Camera::Reset() noexcept
{
	r = 20.0f;
	theta = 0.0f;
	phi = 0.0f;
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;
}