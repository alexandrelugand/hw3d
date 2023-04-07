#pragma once

namespace Bind
{
	class DepthCubeTexture : public Bindable
	{
	public:
		DepthCubeTexture(Graphics& gfx, unsigned int size, unsigned int slot);

		void Bind(Graphics& gfx) noexcpt override;
		std::shared_ptr<OutputOnlyDepthStencil> GetDepthBuffer(size_t index) const;

	protected:
		ComPtr<ID3D11ShaderResourceView> pTextureView;
		std::vector<std::shared_ptr<OutputOnlyDepthStencil>> depthBuffers;

	private:
		unsigned int slot;
	};
}
