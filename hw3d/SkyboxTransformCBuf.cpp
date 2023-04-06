#include "stdafx.h"
#include "SkyboxTransformCBuf.h"

namespace Bind
{
	SkyboxTransformCBuf::SkyboxTransformCBuf(Graphics& gfx, unsigned slot)
		: pVcbuf(std::make_unique<VertexConstantBuffer<Transform>>(gfx, slot))
	{
	}

	void SkyboxTransformCBuf::Bind(Graphics& gfx) noexcpt
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(UpdateBindImpl(gfx, GetTransform(gfx)));
	}

	void SkyboxTransformCBuf::UpdateBindImpl(Graphics& gfx, const Transform& tf) const noexcpt
	{
		pVcbuf->Update(gfx, tf);
		pVcbuf->Bind(gfx);
	}

	SkyboxTransformCBuf::Transform SkyboxTransformCBuf::GetTransform(Graphics& gfx) noexcpt
	{
		return {XMMatrixTranspose(gfx.GetCamera() * gfx.GetProjection())};
	}
}
