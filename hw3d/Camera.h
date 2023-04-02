#pragma once

namespace Entities
{
	class Camera
	{
	public:
		Camera(Graphics& gfx, std::string name, XMFLOAT3 homePos = {0.0f, 0.0f, 0.0f}, float homePitch = 0.0f, float homeYaw = 0.0f, bool tethered = false) noexcept;

		void BindToGraphics(Graphics& gfx) const noexcpt;
		XMMATRIX GetMatrix() const noexcept;
		XMMATRIX GetProjection() const noexcept;

		void SpawnControlWidgets(Graphics& gfx) noexcept;

		void Reset(Graphics& gfx) noexcept;
		void Rotate(float dx, float dy) noexcept;
		void Translate(XMFLOAT3 translation) noexcept;
		XMFLOAT3 GetPos() const noexcept;
		void SetPos(XMFLOAT3 pos) noexcept;
		const std::string& GetName() const noexcept;

		void LinkTechniques(Rgph::RenderGraph& rg);
		void Submit(size_t channelFilter) const;

	private:
		bool tethered;
		std::string name;
		XMFLOAT3 homePos;
		float homePitch;
		float homeYaw;
		XMFLOAT3 pos{};
		float pitch{};
		float yaw{};
		static constexpr float travelSpeed = 12.0f;
		static constexpr float rotationSpeed = 0.004f;
		bool enableCameraIndicator = true;
		bool enableFrustumIndicator = true;
		Projection proj;
		Draw::CameraIndicator indicator;
	};
}
