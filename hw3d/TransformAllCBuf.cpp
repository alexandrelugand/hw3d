#include "stdafx.h"
#include "TransformAllCBuf.h"

namespace Bind
{
	std::unique_ptr<PixelConstantBuffer<TransformCBuf::Transforms>> TransformAllCBuf::pPcbuf;

	TransformAllCBuf::TransformAllCBuf(Graphics& gfx, const Draw::Drawable& parent, UINT VS_slot, UINT PS_slot)
		: TransformCBuf(gfx, parent, VS_slot)
	{
		if (!pPcbuf)
			pPcbuf = std::make_unique<PixelConstantBuffer<Transforms>>(gfx, PS_slot);
	}

	void TransformAllCBuf::Bind(Graphics& gfx) noexcept
	{
		const auto tf = GetTransforms(gfx);
		TransformCBuf::UpdateBindImpl(gfx, tf);
		UpdateBindImpl(gfx, tf);
	}

	void TransformAllCBuf::UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept
	{
		pPcbuf->Update(gfx, tf);
		pPcbuf->Bind(gfx);
	}
}
