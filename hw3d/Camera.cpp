#include "stdafx.h"
#include "Camera.h"

namespace Entities
{
	Camera::Camera(Graphics& gfx, std::string name, XMFLOAT3 homePos, float homePitch, float homeYaw, bool tethered) noexcept
		: tethered(tethered),
		  name(std::move(name)),
		  homePos(homePos),
		  homePitch(homePitch),
		  homeYaw(homeYaw),
		  proj(gfx, 1.0f, 3.0f / 4.0f, 0.5f, 400.0f),
		  indicator(gfx)
	{
		if (tethered)
		{
			pos = homePos;
			indicator.SetPos(pos);
			proj.SetPos(pos);
		}
		Reset(gfx);
	}

	void Camera::BindToGraphics(Graphics& gfx) const noexcpt
	{
		gfx.SetCamera(GetMatrix());
		gfx.SetProjection(proj.GetMatrix());
	}

	XMMATRIX Camera::GetMatrix() const noexcept
	{
		const XMVECTOR forwardBaseVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		// apply the camera rotations to a base vector
		const auto lookVector = XMVector3Transform(forwardBaseVector, XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f));
		// generate camera transform (applied to all objects to arrange them relative
		// to camera position/orientation in world) from cam position and direction
		// camera "top" always faces towards +Y (cannot do a barrel roll => constraint)
		const auto camPosition = XMLoadFloat3(&pos);
		const auto camTarget = camPosition + lookVector;
		return XMMatrixLookAtLH(camPosition, camTarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	}

	XMMATRIX Camera::GetProjection() const noexcept
	{
		return proj.GetMatrix();
	}

	void Camera::SpawnControlWidgets(Graphics& gfx) noexcept
	{
		bool posDirty = false;
		bool rotDirty = false;
		const auto dcheck = [](bool d, bool& carry) { carry = carry || d; };

		if (!tethered)
		{
			ImGui::Text("Position");
			dcheck(ImGui::SliderFloat("X", &pos.x, -80.0f, 80.0f, "%.1f"), posDirty);
			dcheck(ImGui::SliderFloat("Y", &pos.y, -80.0f, 80.0f, "%.1f"), posDirty);
			dcheck(ImGui::SliderFloat("Z", &pos.z, -80.0f, 80.0f, "%.1f"), posDirty);
		}
		ImGui::Text("Rotation");
		dcheck(ImGui::SliderAngle("Pitch", &pitch, 0.995f * -180.0f, 0.995f * 180.0f), rotDirty); //99.5% of angle, not 100% because camera +Y constraint can't be respected
		dcheck(ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f), rotDirty);

		proj.RenderWidgets(gfx);

		ImGui::Checkbox("Camera Indicator", &enableCameraIndicator);
		ImGui::Checkbox("Frustum Indicator", &enableFrustumIndicator);

		if (ImGui::Button("Reset"))
			Reset(gfx);

		if (posDirty)
		{
			indicator.SetPos(pos);
			proj.SetPos(pos);
		}

		if (rotDirty)
		{
			const XMFLOAT3 angle{pitch, yaw, 0.0f};
			indicator.SetRot(angle);
			proj.SetRot(angle);
		}
	}

	void Camera::Reset(Graphics& gfx) noexcept
	{
		if (!tethered)
		{
			pos = homePos;
			indicator.SetPos(pos);
			proj.SetPos(pos);
		}
		pitch = homePitch;
		yaw = homeYaw;

		const XMFLOAT3 angles{pitch, yaw, 0.0f};
		indicator.SetRot(angles);
		proj.SetRot(angles);
		proj.Reset(gfx);
	}

	void Camera::Rotate(float dx, float dy) noexcept
	{
		yaw = wrap_angle(yaw + dx * rotationSpeed);
		pitch = std::clamp(pitch + dy * rotationSpeed, 0.995f * -PI / 2.0f, 0.995f * PI / 2.0f);

		const XMFLOAT3 angles{pitch, yaw, 0.0f};
		indicator.SetRot(angles);
		proj.SetRot(angles);
	}

	void Camera::Translate(XMFLOAT3 translation) noexcept
	{
		if (!tethered)
		{
			XMStoreFloat3(&translation, XMVector3Transform(
				              XMLoadFloat3(&translation),
				              XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
				              XMMatrixScaling(travelSpeed, travelSpeed, travelSpeed)
			              ));

			pos = {
				pos.x + translation.x,
				pos.y + translation.y,
				pos.z + translation.z
			};
			indicator.SetPos(pos);
			proj.SetPos(pos);
		}
	}

	XMFLOAT3 Camera::GetPos() const noexcept
	{
		return pos;
	}

	void Camera::SetPos(XMFLOAT3 pos) noexcept
	{
		this->pos = pos;
		indicator.SetPos(pos);
		proj.SetPos(pos);
	}

	const std::string& Camera::GetName() const noexcept
	{
		return name;
	}

	void Camera::LinkTechniques(Rgph::RenderGraph& rg)
	{
		indicator.LinkTechniques(rg);
		proj.LinkTechniques(rg);
	}

	void Camera::Submit(size_t channelFilter) const
	{
		if (enableCameraIndicator)
		{
			indicator.Submit(channelFilter);
		}
		if (enableFrustumIndicator)
		{
			proj.Submit(channelFilter);
		}
	}
}
