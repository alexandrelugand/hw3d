#include "stdafx.h"
#include "ShadowCameraCBuf.h"

namespace Bind
{
	ShadowCameraCBuf::ShadowCameraCBuf(Graphics& gfx, unsigned int slot)
		: pVcbuf{std::make_unique<VertexConstantBuffer<Transform>>(gfx, slot)}
	{
	}

	void ShadowCameraCBuf::Bind(Graphics& gfx) noexcpt
	{
		pVcbuf->Bind(gfx);
	}

	void ShadowCameraCBuf::Update(Graphics& gfx) const
	{
		const auto pos = pCamera->GetPos();
		const Transform t{
			XMMatrixTranspose(
				XMMatrixTranslation(-pos.x, -pos.y, -pos.z)
			)
		};
		pVcbuf->Update(gfx, t);
	}

	void ShadowCameraCBuf::SetCamera(const Entities::Camera* p) noexcept
	{
		pCamera = p;
	}
}
