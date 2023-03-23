#include "stdafx.h"
#include "Mesh.h"

namespace Entities
{
	Mesh::Mesh(Graphics& gfx)
		: DrawableObject(gfx)
	{
	}

	XMMATRIX Mesh::GetTransform() const noexcept
	{
		return XMLoadFloat4x4(&transform);
	}

	void Mesh::Submit(FrameCommander& frame, FXMMATRIX accumulatedTransform) const noexcept(!true)
	{
		XMStoreFloat4x4(&transform, accumulatedTransform);
		Drawable::Submit(frame);
	}
}
