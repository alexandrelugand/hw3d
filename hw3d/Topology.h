#pragma once

class Topology : public Bindable
{
public:
	Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(Graphics& gfx) noexcpt override;

private:
	D3D11_PRIMITIVE_TOPOLOGY type;
};
