#include "stdafx.h"
#include "CubeTargetTexture.h"

namespace Bind
{
	CubeTargetTexture::CubeTargetTexture(Graphics& gfx, unsigned width, unsigned height, unsigned slot, DXGI_FORMAT format)
		: slot(slot)
	{
		INFOMAN(gfx);

		// texture descriptor
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 6;
		textureDesc.Format = format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		// create the texture resource
		ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(
			&textureDesc, nullptr, &pTexture
		));

		// create the resource view on the texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
			pTexture.Get(), &srvDesc, &pTextureView
		));

		// make render target resources for capturing shadow map
		for (UINT face = 0; face < 6; face++)
		{
			renderTargets.push_back(std::make_shared<OutputOnlyRenderTarget>(gfx, pTexture.Get(), face));
		}
	}

	void CubeTargetTexture::Bind(Graphics& gfx) noexcpt
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShaderResources(slot, 1u, pTextureView.GetAddressOf()));
	}

	std::shared_ptr<OutputOnlyRenderTarget> CubeTargetTexture::GetRenderTarget(size_t index) const
	{
		return renderTargets[index];
	}
}
