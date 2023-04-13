#pragma once

namespace Bind
{
	class CubeTargetTexture : public Bindable
	{
	public:
		CubeTargetTexture(Graphics& gfx, unsigned int width, unsigned int height, unsigned int slot, DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM);

		void Bind(Graphics& gfx) noexcpt override;
		std::shared_ptr<OutputOnlyRenderTarget> GetRenderTarget(size_t index) const;

	protected:
		ComPtr<ID3D11ShaderResourceView> pTextureView;
		std::vector<std::shared_ptr<OutputOnlyRenderTarget>> renderTargets;

	private:
		unsigned int slot;
	};
}
