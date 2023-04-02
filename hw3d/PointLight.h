#pragma once
#include "SolidSphere.h"

namespace Entities
{
	class PointLight
	{
	public:
		PointLight(Graphics& gfx, XMFLOAT3 pos = {10.0f, 9.0f, 2.5f}, float pitch = 0.0f, float yaw = 0.0f, float radius = 0.5f, XMFLOAT3 color = {1.0f, 1.0f, 1.0f});
		void SpawnControlWindow() noexcept;
		void Reset() noexcept;
		void Submit(size_t channelFilter) const noexcpt;
		void Bind(Graphics& gfx, FXMMATRIX view) const noexcept;
		void LinkTechniques(Rgph::RenderGraph& rg) const;
		std::shared_ptr<Camera> ShareCamera() const noexcept;

	private:
		struct PointLightCBuf
		{
			alignas(16) XMFLOAT3 pos;
			alignas(16) XMFLOAT3 ambient;
			alignas(16) XMFLOAT3 diffuseColor;
			float diffuseIntensity;
			float attConst;
			float attLin;
			float attQuad;
		};

		PointLightCBuf home{};
		PointLightCBuf cbData{};
		mutable Draw::SolidSphere mesh;
		mutable Bind::PixelConstantBuffer<PointLightCBuf> cbuf;
		std::shared_ptr<Camera> pCamera;
	};
}
