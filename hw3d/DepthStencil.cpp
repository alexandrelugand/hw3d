#include "stdafx.h"
#include "DepthStencil.h"

namespace Bind
{
	DXGI_FORMAT MapUsageTypeLess(DepthStencil::Usage usage)
	{
		switch (usage)
		{
		case DepthStencil::Usage::DepthStencil:
			return DXGI_FORMAT_R24G8_TYPELESS;
		case DepthStencil::Usage::ShadowDepth:
			return DXGI_FORMAT_R32_TYPELESS;
		default:
			throw std::runtime_error{"Base usage for Typeless format map in DepthStencil"};
		}
	}

	DXGI_FORMAT MapUsageTyped(DepthStencil::Usage usage)
	{
		switch (usage)
		{
		case DepthStencil::Usage::DepthStencil:
			return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case DepthStencil::Usage::ShadowDepth:
			return DXGI_FORMAT_D32_FLOAT;
		default:
			throw std::runtime_error{"Base usage for Typed format map in DepthStencil"};
		}
	}

	DXGI_FORMAT MapUsageColored(DepthStencil::Usage usage)
	{
		switch (usage)
		{
		case DepthStencil::Usage::DepthStencil:
			return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		case DepthStencil::Usage::ShadowDepth:
			return DXGI_FORMAT_R32_FLOAT;
		default:
			throw std::runtime_error{"Base usage for Colored format map in DepthStencil"};
		}
	}

	void DepthStencil::BindAsBuffer(Graphics& gfx) noexcpt
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->OMSetRenderTargets(0, nullptr, pDepthStencilView.Get()));
	}

	void DepthStencil::BindAsBuffer(Graphics& gfx, BufferResource* buffer) noexcpt
	{
		assert(dynamic_cast<RenderTarget*>(buffer) != nullptr);
		BindAsBuffer(gfx, dynamic_cast<RenderTarget*>(buffer));
	}

	void DepthStencil::BindAsBuffer(Graphics& gfx, RenderTarget* rt) noexcpt
	{
		rt->BindAsBuffer(gfx, this);
	}

	void DepthStencil::Clear(Graphics& gfx) noexcpt
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u));
	}

	Surface DepthStencil::ToSurface(Graphics& gfx, bool linearlize) const
	{
		INFOMAN(gfx);

		// get info about the stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC srcViewDesc{};
		pDepthStencilView->GetDesc(&srcViewDesc);

		// creating a temp texture compatible with the source, but with CPU read access
		ComPtr<ID3D11Resource> pResSource;
		pDepthStencilView->GetResource(&pResSource);
		ComPtr<ID3D11Texture2D> pTexSource;
		pResSource.As(&pTexSource);

		D3D11_TEXTURE2D_DESC srcTextureDesc{};
		pTexSource->GetDesc(&srcTextureDesc);

		D3D11_TEXTURE2D_DESC tmpTextureDesc = srcTextureDesc;
		tmpTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		tmpTextureDesc.Usage = D3D11_USAGE_STAGING;
		tmpTextureDesc.BindFlags = 0;
		tmpTextureDesc.MiscFlags = 0;
		tmpTextureDesc.ArraySize = 1;

		ComPtr<ID3D11Texture2D> pTexTemp;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&tmpTextureDesc, nullptr, &pTexTemp));

		// copy texture contents
		if (srcViewDesc.ViewDimension == D3D11_DSV_DIMENSION_TEXTURE2DARRAY)
		{
			// source is actually inside a cubemap texture, use view info to find the correct slice and copy subresource
			GFX_THROW_INFO_ONLY(GetContext(gfx)->CopySubresourceRegion(pTexTemp.Get(), 0, 0, 0, 0, pTexSource.Get(), srcViewDesc.Texture2DArray.FirstArraySlice, nullptr));
		}
		else
		{
			GFX_THROW_INFO_ONLY(GetContext(gfx)->CopyResource(pTexTemp.Get(), pTexSource.Get()));
		}

		// create Surface and copy from temp texture to it
		const auto width = GetWidth();
		const auto height = GetHeight();
		Surface s{width, height};
		D3D11_MAPPED_SUBRESOURCE msr{};
		GFX_THROW_INFO(GetContext(gfx)->Map(pTexTemp.Get(), 0, D3D11_MAP_READ, 0, &msr));
		const auto pSrcBytes = static_cast<const char*>(msr.pData);
		for (unsigned int y = 0; y < height; y++)
		{
			struct Pixel
			{
				char data[4];
			};
			const auto pSrcRow = reinterpret_cast<const Pixel*>(pSrcBytes + msr.RowPitch * static_cast<size_t>(y));
			for (unsigned int x = 0; x < width; x++)
			{
				if (srcTextureDesc.Format == DXGI_FORMAT_R24G8_TYPELESS)
				{
					const auto raw = 0xFFFFFF & *reinterpret_cast<const unsigned int*>(pSrcRow + x);
					if (linearlize)
					{
						const auto normalized = static_cast<float>(raw) / static_cast<float>(0xFFFFFF);
						const auto linearized = 0.01f / (1.01f - normalized);
						const auto channel = static_cast<unsigned char>(linearized * 255.0f);
						s.PutPixel(x, y, {channel, channel, channel});
					}
					else
					{
						const unsigned char channel = raw >> 16;
						s.PutPixel(x, y, {channel, channel, channel});
					}
				}
				else if (srcTextureDesc.Format == DXGI_FORMAT_R32_TYPELESS)
				{
					const auto raw = *reinterpret_cast<const float*>(pSrcRow + x);
					if (linearlize)
					{
						const auto linearized = 0.01f / (1.01f - raw);
						const auto channel = static_cast<unsigned char>(linearized * 255.0f);
						s.PutPixel(x, y, {channel, channel, channel});
					}
					else
					{
						const auto channel = static_cast<unsigned char>(raw * 255.0f);
						s.PutPixel(x, y, {channel, channel, channel});
					}
				}
				else
				{
					throw std::runtime_error("Bad format in Depth Stencil for conversion to Surface");
				}
			}
		}
		GFX_THROW_INFO_ONLY(GetContext(gfx)->Unmap(pTexTemp.Get(), 0));
		return s;
	}

	unsigned DepthStencil::GetWidth() const
	{
		return width;
	}

	unsigned DepthStencil::GetHeight() const
	{
		return height;
	}

	DepthStencil::DepthStencil(Graphics& gfx, ComPtr<ID3D11Texture2D> pTexture, UINT face)
	{
		INFOMAN(gfx);

		D3D11_TEXTURE2D_DESC descTex = {};
		pTexture->GetDesc(&descTex);
		width = descTex.Width;
		height = descTex.Height;

		// create target view of depth stensil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC descView;
		descView.Format = DXGI_FORMAT_D32_FLOAT;
		descView.Flags = 0;
		descView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		descView.Texture2DArray.MipSlice = 0;
		descView.Texture2DArray.ArraySize = 1;
		descView.Texture2DArray.FirstArraySlice = face;

		GFX_THROW_INFO(GetDevice(gfx)->CreateDepthStencilView(
			pTexture.Get(), &descView, &pDepthStencilView
		));
	}

	DepthStencil::DepthStencil(Graphics& gfx, UINT width, UINT height, bool canBindShaderInput, Usage usage)
		: width(width),
		  height(height)
	{
		INFOMAN(gfx);

		// create depth stencil texture
		ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1u;
		descDepth.ArraySize = 1u;
		descDepth.Format = MapUsageTypeLess(usage);
		descDepth.SampleDesc.Count = 1u;
		descDepth.SampleDesc.Quality = 0u;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | (canBindShaderInput ? D3D11_BIND_SHADER_RESOURCE : 0);
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

		// create target view of depth stencil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC descView;
		descView.Format = MapUsageTyped(usage);
		descView.Flags = 0;
		descView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descView.Texture2D.MipSlice = 0;
		GFX_THROW_INFO(GetDevice(gfx)->CreateDepthStencilView(
			pDepthStencil.Get(), &descView, &pDepthStencilView
		));
	}

	ShaderInputDepthStencil::ShaderInputDepthStencil(Graphics& gfx, UINT slot, Usage usage)
		: ShaderInputDepthStencil(gfx, gfx.GetWidth(), gfx.GetHeight(), slot, usage)
	{
	}

	ShaderInputDepthStencil::ShaderInputDepthStencil(Graphics& gfx, UINT width, UINT height, UINT slot, Usage usage)
		: DepthStencil(gfx, width, height, true, usage), slot(slot)
	{
		INFOMAN(gfx);

		ComPtr<ID3D11Resource> pRes;
		pDepthStencilView->GetResource(&pRes);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = MapUsageColored(usage);
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
			pRes.Get(), &srvDesc, &pShaderResourceView
		));
	}

	void ShaderInputDepthStencil::Bind(Graphics& gfx) noexcpt
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShaderResources(slot, 1u, pShaderResourceView.GetAddressOf()));
	}

	OutputOnlyDepthStencil::OutputOnlyDepthStencil(Graphics& gfx, ComPtr<ID3D11Texture2D> pTexture, UINT face)
		: DepthStencil(gfx, std::move(pTexture), face)
	{
	}

	OutputOnlyDepthStencil::OutputOnlyDepthStencil(Graphics& gfx)
		: OutputOnlyDepthStencil(gfx, gfx.GetWidth(), gfx.GetHeight())
	{
	}

	OutputOnlyDepthStencil::OutputOnlyDepthStencil(Graphics& gfx, UINT width, UINT height)
		: DepthStencil(gfx, width, height, false, Usage::DepthStencil)
	{
	}

	void OutputOnlyDepthStencil::Bind(Graphics& gfx) noexcpt
	{
		assert("OutputOnlyDepthStencil cannot be bound as shader input" && false);
	}
}
