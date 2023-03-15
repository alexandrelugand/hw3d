#pragma once

namespace Bind
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(Graphics& gfx, const std::wstring& path);
		void Bind(Graphics& gfx) noexcpt override;

	private:
		ComPtr<ID3D11PixelShader> pPixelShader;
	};
}
