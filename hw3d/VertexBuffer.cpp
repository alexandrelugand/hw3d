#include "stdafx.h"
#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& gfx) noexcpt
{
	const UINT offset = 0u;
	INFOMAN_NOHR(gfx);
	GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset));
}
