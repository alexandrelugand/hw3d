#include "stdafx.h"
#include "TransformAllCBuf.h"

namespace Bind
{
	std::unique_ptr<PixelConstantBuffer<TransformCBuf::Transforms>> TransformAllCBuf::pPcbuf;

	TransformAllCBuf::TransformAllCBuf(Graphics& gfx, unsigned int VS_slot, unsigned int PS_slot)
		: TransformCBuf(gfx, VS_slot)
	{
		if (!pPcbuf)
			pPcbuf = std::make_unique<PixelConstantBuffer<Transforms>>(gfx, PS_slot);
	}

	void TransformAllCBuf::Bind(Graphics& gfx) noexcpt
	{
		const auto tf = GetTransforms(gfx);
		TransformCBuf::UpdateBindImpl(gfx, tf);
		UpdateBindImpl(gfx, tf);
	}

	std::unique_ptr<CloningBindable> TransformAllCBuf::Clone() const noexcept
	{
		return std::make_unique<TransformAllCBuf>(*this);
	}

	void TransformAllCBuf::UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcpt
	{
		pPcbuf->Update(gfx, tf);
		pPcbuf->Bind(gfx);
	}
}
