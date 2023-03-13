#pragma once

class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode);
	void Bind(Graphics& gfx) noexcpt override;

private:
	ComPtr<ID3D11InputLayout> pInputLayout;
};