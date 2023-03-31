#pragma once

namespace Entities
{
	class Camera
	{
	public:
		Camera(XMFLOAT3 homePos = {0.0f, 0.0f, 0.0f}, float homePitch = 0.0f, float homeYaw = 0.0f) noexcept;
		XMMATRIX GetMatrix() const noexcept;
		void SpawnControlWindow() noexcept;
		void Reset() noexcept;
		void Rotate(float dx, float dy) noexcept;
		void Translate(XMFLOAT3 translation) noexcept;
		XMFLOAT3 GetPos() const noexcept;

	private:
		XMFLOAT3 homePos;
		float homePitch;
		float homeYaw;
		XMFLOAT3 pos;
		float pitch;
		float yaw;
		static constexpr float travelSpeed = 12.0f;
		static constexpr float rotationSpeed = 0.004f;
	};
}
