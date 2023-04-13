#pragma once

namespace Bind
{
	class RenderTarget : public Bindable, public BufferResource
	{
	public:
		void BindAsBuffer(Graphics& gfx) noexcpt override;
		void BindAsBuffer(Graphics& gfx, BufferResource* buffer) noexcpt override;
		void BindAsBuffer(Graphics& gfx, DepthStencil* depthStencil) noexcpt;
		void Clear(Graphics& gfx) noexcpt override;
		void Clear(Graphics& gfx, const std::array<float, 4>& color) const noexcpt;

		UINT GetWidth() const noexcept;
		UINT GetHeight() const noexcept;

		Surface ToSurface(Graphics& gfx) const;
		void Dumpy(Graphics& gfx, const std::string& path) const;

	protected:
		RenderTarget(Graphics& gfx, ID3D11Texture2D* pTexture, std::optional<unsigned int> face);
		RenderTarget(Graphics& gfx, UINT width, UINT height);
		UINT width;
		UINT height;
		ComPtr<ID3D11RenderTargetView> pTargetView = nullptr;

	private:
		void BindAsBuffer(Graphics& gfx, ID3D11DepthStencilView* pDepthStencilView) noexcpt;
		std::pair<ComPtr<ID3D11Texture2D>, D3D11_TEXTURE2D_DESC> MakeStaging(Graphics& gfx) const;
	};

	class ShaderInputRenderTarget : public RenderTarget
	{
	public:
		ShaderInputRenderTarget(Graphics& gfx, UINT width, UINT height, UINT slot);
		void Bind(Graphics& gfx) noexcpt override;

	private:
		UINT slot;
		ComPtr<ID3D11ShaderResourceView> pShaderResourceView = nullptr;
	};

	class OutputOnlyRenderTarget : public RenderTarget
	{
		friend class Graphics;

	public:
		OutputOnlyRenderTarget(Graphics& gfx, ID3D11Texture2D* pTexture, std::optional<unsigned int> face = {});
		void Bind(Graphics& gfx) noexcpt override;
	};
}
