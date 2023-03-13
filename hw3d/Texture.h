#pragma once

class Texture : public Bindable
{
public:
	Texture(Graphics& gfx, const class Surface& s);
	void Bind(Graphics& gfx) noexcpt override;

protected:
	ComPtr<ID3D11ShaderResourceView> pTextureView;
};
