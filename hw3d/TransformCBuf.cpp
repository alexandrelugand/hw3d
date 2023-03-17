#include "stdafx.h"
#include "TransformCBuf.h"

namespace Bind
{
	std::unique_ptr<VertexConstantBuffer<TransformCBuf::Transforms>> TransformCBuf::pVcbuf;

	TransformCBuf::TransformCBuf(Graphics& gfx, const Draw::Drawable& parent, UINT slot)
		: parent(parent)
	{
		if (!pVcbuf)
			pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
	}

	void TransformCBuf::Bind(Graphics& gfx) noexcept
	{
		UpdateBindImpl(gfx, GetTransforms(gfx));
	}

	void TransformCBuf::UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept
	{
		pVcbuf->Update(gfx, tf);
		pVcbuf->Bind(gfx);
	}

	TransformCBuf::Transforms TransformCBuf::GetTransforms(Graphics& gfx) noexcept
	{
		const auto modelView = parent.GetTransform() * gfx.GetCamera();
		return {
			XMMatrixTranspose(modelView),
			XMMatrixTranspose(
				modelView *
				gfx.GetProjection()
			)
		};
	}
}
