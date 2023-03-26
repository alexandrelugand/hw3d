#pragma once

class DepthStencil : public GraphicsResource
{
	friend class RenderTarget;
	friend class Graphics;

public:
	DepthStencil(Graphics& gfx, UINT width, UINT height);
	void BindAsDepthStencil(Graphics& gfx) const noexcept;
	void Clear(Graphics& gfx) const noexcept;

private:
	ComPtr<ID3D11DepthStencilView> pDepthStencilView = nullptr;
};
