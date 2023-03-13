#pragma once

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, const std::wstring& path);
	void Bind(Graphics& gfx) noexcpt override;
	ID3DBlob* GetBytecode() const noexcept;

private:
	ComPtr<ID3DBlob> pBlob;
	ComPtr<ID3D11VertexShader> pVertexShader;
};
