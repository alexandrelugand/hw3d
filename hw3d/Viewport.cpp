#include "stdafx.h"
#include "Viewport.h"

namespace Bind
{
	Viewport::Viewport(Graphics& gfx)
		: Viewport(gfx, static_cast<float>(gfx.GetWidth()), static_cast<float>(gfx.GetHeight()))
	{
	}

	Viewport::Viewport(Graphics& gfx, float width, float height)
	{
		vp.Width = width;
		vp.Height = height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
	}

	void Viewport::Bind(Graphics& gfx) noexcept(!true)
	{
		INFOMAN_NOHR(gfx);
		GetContext(gfx)->RSSetViewports(1u, &vp);
	}
}
