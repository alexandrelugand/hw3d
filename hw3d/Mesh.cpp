#include "stdafx.h"
#include "Mesh.h"

namespace Entities
{
	Mesh::Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs)
		: DrawableObject(gfx)
	{
		AddBind(Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		for (auto& pb : bindPtrs)
		{
			AddBind(std::move(pb));
		}

		AddBind(std::make_shared<Bind::TransformCBuf>(gfx, *this));
	}

	void Mesh::Draw(Graphics& gfx, FXMMATRIX accumulatedTransform) const noexcpt
	{
		XMStoreFloat4x4(&transform, accumulatedTransform);
		Drawable::Draw(gfx);
	}

	XMMATRIX Mesh::GetTransform() const noexcept
	{
		return XMLoadFloat4x4(&transform);
	}
}
