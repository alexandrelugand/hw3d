#include "stdafx.h"
#include "ShadowRasterizer.h"

namespace Bind
{
	ShadowRasterizer::ShadowRasterizer(Graphics& gfx, int depthBias, float slopeBias, float clamp)
	{
		ChangeDepthBiasParameters(gfx, depthBias, slopeBias, clamp);
	}

	void ShadowRasterizer::Bind(Graphics& gfx) noexcpt
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->RSSetState(pRasterizer.Get()));
	}

	void ShadowRasterizer::ChangeDepthBiasParameters(Graphics& gfx, int depthBias, float slopeBias, float clamp)
	{
		this->depthBias = depthBias;
		this->slopeBias = slopeBias;
		this->clamp = clamp;

		INFOMAN(gfx);

		D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
		rasterDesc.DepthBias = depthBias;
		rasterDesc.SlopeScaledDepthBias = slopeBias;
		rasterDesc.DepthBiasClamp = clamp;

		GFX_THROW_INFO(GetDevice(gfx)->CreateRasterizerState(&rasterDesc, &pRasterizer));
	}

	int ShadowRasterizer::GetDepthBias() const
	{
		return depthBias;
	}

	float ShadowRasterizer::GetSlopeBias() const
	{
		return slopeBias;
	}

	float ShadowRasterizer::GetClamp() const
	{
		return clamp;
	}
}
