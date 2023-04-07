#include "stdafx.h"
#include "DepthCubeTexture.h"

namespace Bind
{
	DepthCubeTexture::DepthCubeTexture(Graphics& gfx, unsigned int size, unsigned int slot)
		: slot(slot)
	{
		INFOMAN(gfx);

		// texture descriptor
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = size;
		textureDesc.Height = size;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 6;
		textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		// create the texture resource
		ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(
			&textureDesc, nullptr, &pTexture
		));

		// create the resource view on the texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
			pTexture.Get(), &srvDesc, &pTextureView
		));

		// make depth buffer resources for capturing shadow map
		for (UINT face = 0; face < 6; face++)
		{
			depthBuffers.push_back(std::make_shared<OutputOnlyDepthStencil>(gfx, pTexture, face));
		}
	}

	void DepthCubeTexture::Bind(Graphics& gfx) noexcept(!true)
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShaderResources(slot, 1u, pTextureView.GetAddressOf()));
	}

	std::shared_ptr<OutputOnlyDepthStencil> DepthCubeTexture::GetDepthBuffer(size_t index) const
	{
		return depthBuffers[index];
	}
}
