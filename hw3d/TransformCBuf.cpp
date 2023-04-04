#include "stdafx.h"
#include "TransformCBuf.h"

namespace Bind
{
	std::unique_ptr<VertexConstantBuffer<TransformCBuf::Transforms>> TransformCBuf::pVcbuf;

	TransformCBuf::TransformCBuf(Graphics& gfx, unsigned int slot)
	{
		if (!pVcbuf)
			pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
	}

	void TransformCBuf::Bind(Graphics& gfx) noexcpt
	{
		UpdateBindImpl(gfx, GetTransforms(gfx));
	}

	void TransformCBuf::InitializeParentReferences(const Draw::Drawable& parent) noexcept
	{
		pParent = &parent;
	}

	std::unique_ptr<CloningBindable> TransformCBuf::Clone() const noexcept
	{
		return std::make_unique<TransformCBuf>(*this);
	}

	void TransformCBuf::UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcpt
	{
		pVcbuf->Update(gfx, tf);
		pVcbuf->Bind(gfx);
	}

	TransformCBuf::Transforms TransformCBuf::GetTransforms(Graphics& gfx) noexcpt
	{
		assert(pParent != nullptr);
		const auto model = pParent->GetTransform();
		const auto modelView = model * gfx.GetCamera();
		return {
			XMMatrixTranspose(model),
			XMMatrixTranspose(modelView),
			XMMatrixTranspose(
				modelView *
				gfx.GetProjection()
			)
		};
	}
}
