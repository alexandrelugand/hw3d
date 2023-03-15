#pragma once
#include "SolidSphere.h"

namespace Entities
{
	class PointLight
	{
	public:
		PointLight(Graphics& gfx, float radius = 0.5f, XMFLOAT3 color = {1.0f, 1.0f, 1.0f});
		void SpawnControlWindow() noexcept;
		void Reset() noexcept;
		void Draw(Graphics& gfx) const noexcpt;
		void Bind(Graphics& gfx, FXMMATRIX view) const noexcept;

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

		PointLightCBuf cbData;
		mutable Draw::SolidSphere mesh;
		mutable Bind::PixelConstantBuffer<PointLightCBuf> cbuf;
	};
}
