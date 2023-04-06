#pragma once

namespace Bind
{
	class CubeTexture : public Bindable
	{
	public:
		CubeTexture(Graphics& gfx, const std::string& path, UINT slot);
		void Bind(Graphics& gfx) noexcpt override;

	protected:
		std::string path;
		ComPtr<ID3D11ShaderResourceView> pTextureView;

	private:
		unsigned int slot;
	};
}
