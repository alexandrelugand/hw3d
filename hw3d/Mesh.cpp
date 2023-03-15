#include "stdafx.h"
#include "Mesh.h"

namespace Entities
{
	Mesh::Mesh(Graphics& gfx, std::vector<std::unique_ptr<Bind::Bindable>> bindPtrs)
	{
		if (!IsStaticInitialized())
		{
			AddStaticBind(std::make_unique<Bind::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}

		for (auto& pb : bindPtrs)
		{
			if (auto pi = dynamic_cast<Bind::IndexBuffer*>(pb.get()))
			{
				AddIndexBuffer(std::unique_ptr<Bind::IndexBuffer>{pi});
				pb.release(); //release last smart pointer holding index buffer pointer
			}
			else
			{
				AddBind(std::move(pb));
			}
		}

		AddBind(std::make_unique<Bind::TransformCBuf>(gfx, *this));
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
