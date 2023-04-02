#include "stdafx.h"
#include "Mesh.h"

namespace Entities
{
	Mesh::Mesh(Graphics& gfx, const Material& mat, const aiMesh& mesh, float scale)
		: DrawableObject(gfx, mat, mesh, scale)
	{
		name = "Mesh";
	}

	XMMATRIX Mesh::GetTransform() const noexcept
	{
		return XMLoadFloat4x4(&transform);
	}

	void Mesh::Submit(size_t channelFilter, FXMMATRIX accumulatedTransform) const noexcpt
	{
		XMStoreFloat4x4(&transform, accumulatedTransform);
		Drawable::Submit(channelFilter);
	}
}
