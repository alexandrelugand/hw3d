#include "stdafx.h"
#include "TransformCBuf.h"

std::unique_ptr<VertexConstantBuffer<TransformCBuf::Transforms>> TransformCBuf::pVcbuf;

TransformCBuf::TransformCBuf(Graphics& gfx, const Drawable& parent, UINT slot)
	: parent(parent)
{
	if (!pVcbuf)
		pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
}

void TransformCBuf::Bind(Graphics& gfx) noexcept
{
	const auto modelView = parent.GetTransform() * gfx.GetCamera();
	const Transforms tf =
	{
		XMMatrixTranspose(modelView),
		XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
		)
	};
	pVcbuf->Update(gfx, tf);
	pVcbuf->Bind(gfx);
}
