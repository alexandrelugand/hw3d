#include "stdafx.h"
#include "RenderTarget.h"

namespace Bind
{
	void RenderTarget::BindAsBuffer(Graphics& gfx) noexcpt
	{
		ID3D11DepthStencilView* const null = nullptr;
		BindAsBuffer(gfx, null);
	}

	void RenderTarget::BindAsBuffer(Graphics& gfx, BufferResource* buffer) noexcpt
	{
		assert(dynamic_cast<DepthStencil*>(buffer) != nullptr);
		BindAsBuffer(gfx, static_cast<DepthStencil*>(buffer));
	}

	void RenderTarget::BindAsBuffer(Graphics& gfx, DepthStencil* depthStencil) noexcpt
	{
		BindAsBuffer(gfx, depthStencil ? depthStencil->pDepthStencilView.Get() : nullptr);
	}

	void RenderTarget::Clear(Graphics& gfx) noexcpt
	{
		Clear(gfx, {0.0f, 0.0f, 0.0f, 0.0f});
	}

	void RenderTarget::Clear(Graphics& gfx, const std::array<float, 4>& color) const noexcpt
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->ClearRenderTargetView(pTargetView.Get(), color.data()));
	}

	UINT RenderTarget::GetWidth() const noexcept
	{
		return width;
	}

	UINT RenderTarget::GetHeight() const noexcept
	{
		return height;
	}

	Surface RenderTarget::ToSurface(Graphics& gfx) const
	{
		INFOMAN(gfx);

		auto [pTexTemp, desc] = MakeStaging(gfx);

		if (desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM)
			throw std::runtime_error("ToSurface in RenderTarget on bad dxgi format");

		// create Surface and copy from temp texture to it
		const auto width = GetWidth();
		const auto height = GetHeight();
		Surface s{width, height};
		D3D11_MAPPED_SUBRESOURCE msr = {};
		GFX_THROW_INFO(GetContext(gfx)->Map(pTexTemp.Get(), 0, D3D11_MAP::D3D11_MAP_READ, 0, &msr));

		const auto pSrcBytes = static_cast<const char*>(msr.pData);
		for (unsigned int y = 0; y < height; y++)
		{
			const auto pSrcRow = reinterpret_cast<const Surface::Color*>(pSrcBytes + msr.RowPitch * static_cast<size_t>(y));
			for (unsigned int x = 0; x < width; x++)
			{
				s.PutPixel(x, y, *(pSrcRow + x));
			}
		}
		GFX_THROW_INFO_ONLY(GetContext(gfx)->Unmap(pTexTemp.Get(), 0));

		return s;
	}

	void RenderTarget::Dumpy(Graphics& gfx, const std::string& path) const
	{
		INFOMAN(gfx);

		auto [pTexTemp, srcTextureDesc] = MakeStaging(gfx);

		// mapping texture and preparing vector
		const auto width = GetWidth();
		const auto height = GetHeight();
		std::vector<float> arr;
		arr.reserve(width * height);
		D3D11_MAPPED_SUBRESOURCE msr = {};
		GFX_THROW_INFO(GetContext(gfx)->Map(pTexTemp.Get(), 0, D3D11_MAP::D3D11_MAP_READ, 0, &msr));
		auto pSrcBytes = static_cast<const char*>(msr.pData);

		UINT nElements;

		// flatten texture elements		
		if (srcTextureDesc.Format == DXGI_FORMAT_R32_FLOAT)
		{
			nElements = 1;
			for (unsigned int y = 0; y < height; y++)
			{
				auto pSrcRow = reinterpret_cast<const float*>(pSrcBytes + msr.RowPitch * static_cast<size_t>(y));
				for (unsigned int x = 0; x < width; x++)
				{
					arr.push_back(pSrcRow[x]);
				}
			}
		}
		else if (srcTextureDesc.Format == DXGI_FORMAT_R32G32_FLOAT)
		{
			nElements = 2;
			struct Element
			{
				float r;
				float g;
			};
			for (unsigned int y = 0; y < height; y++)
			{
				auto pSrcRow = reinterpret_cast<const Element*>(pSrcBytes + msr.RowPitch * static_cast<size_t>(y));
				for (unsigned int x = 0; x < width; x++)
				{
					arr.push_back(pSrcRow[x].r);
					arr.push_back(pSrcRow[x].g);
				}
			}
		}
		else
		{
			GFX_THROW_INFO_ONLY(GetContext(gfx)->Unmap(pTexTemp.Get(), 0));
			throw std::runtime_error{"Bad format in RenderTarget for dumpy"};
		}
		GFX_THROW_INFO_ONLY(GetContext(gfx)->Unmap(pTexTemp.Get(), 0));

		// dump to numpy array
		cnpy::npy_save(path, arr.data(), {height, width, nElements});
	}

	RenderTarget::RenderTarget(Graphics& gfx, ID3D11Texture2D* pTexture, std::optional<unsigned int> face)
	{
		INFOMAN(gfx);

		// get information from texture about dimensions
		D3D11_TEXTURE2D_DESC textureDesc;
		pTexture->GetDesc(&textureDesc);
		width = textureDesc.Width;
		height = textureDesc.Height;

		// create the target view on the texture
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		rtvDesc.Format = textureDesc.Format;

		if (face.has_value())
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			rtvDesc.Texture2DArray.ArraySize = 1;
			rtvDesc.Texture2DArray.FirstArraySlice = *face;
			rtvDesc.Texture2DArray.MipSlice = 0;
		}
		else
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D = D3D11_TEX2D_RTV{0};
		}

		GFX_THROW_INFO(GetDevice(gfx)->CreateRenderTargetView(
			pTexture, &rtvDesc, &pTargetView
		));
	}

	RenderTarget::RenderTarget(Graphics& gfx, UINT width, UINT height)
		: width(width), height(height)
	{
		INFOMAN(gfx);

		// create texture resource
		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // never do we not want to bind offscreen RTs as inputs
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(
			&textureDesc, nullptr, &pTexture
		));

		// create the target view on the texture
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		rtvDesc.Format = textureDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D = D3D11_TEX2D_RTV{0};
		GFX_THROW_INFO(GetDevice(gfx)->CreateRenderTargetView(
			pTexture.Get(), &rtvDesc, &pTargetView
		));
	}

	void RenderTarget::BindAsBuffer(Graphics& gfx, ID3D11DepthStencilView* pDepthStencilView) noexcpt
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->OMSetRenderTargets(1, pTargetView.GetAddressOf(), pDepthStencilView));

		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = static_cast<float>(width);
		vp.Height = static_cast<float>(height);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		GFX_THROW_INFO_ONLY(GetContext(gfx)->RSSetViewports(1u, &vp));
	}

	std::pair<ComPtr<ID3D11Texture2D>, D3D11_TEXTURE2D_DESC> RenderTarget::MakeStaging(Graphics& gfx) const
	{
		INFOMAN(gfx);

		// get info about the stencil view
		D3D11_RENDER_TARGET_VIEW_DESC srcViewDesc{};
		pTargetView->GetDesc(&srcViewDesc);

		// creating a temp texture compatible with the source, but with CPU read access
		ComPtr<ID3D11Resource> pResSource;
		pTargetView->GetResource(&pResSource);

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
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(
			&tmpTextureDesc, nullptr, &pTexTemp
		));

		// copy texture contents
		if (srcViewDesc.ViewDimension == D3D11_RTV_DIMENSION_TEXTURE2DARRAY)
		{
			// source is actually inside a cubemap texture, use view info to find the correct slice and copy subresource
			GFX_THROW_INFO_ONLY(GetContext(gfx)->CopySubresourceRegion(pTexTemp.Get(), 0, 0, 0, 0, pTexSource.Get(), srcViewDesc.Texture2DArray.FirstArraySlice, nullptr));
		}
		else
		{
			GFX_THROW_INFO_ONLY(GetContext(gfx)->CopyResource(pTexTemp.Get(), pTexSource.Get()));
		}

		return {std::move(pTexTemp), srcTextureDesc};
	}

	ShaderInputRenderTarget::ShaderInputRenderTarget(Graphics& gfx, UINT width, UINT height, UINT slot)
		: RenderTarget(gfx, width, height), slot(slot)
	{
		INFOMAN(gfx);

		ComPtr<ID3D11Resource> pRes;
		pTargetView->GetResource(&pRes);

		// create the resource view on the texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
			pRes.Get(), &srvDesc, &pShaderResourceView
		));
	}

	void ShaderInputRenderTarget::Bind(Graphics& gfx) noexcpt
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShaderResources(slot, 1u, pShaderResourceView.GetAddressOf()));
	}

	OutputOnlyRenderTarget::OutputOnlyRenderTarget(Graphics& gfx, ID3D11Texture2D* pTexture, std::optional<unsigned int> face)
		: RenderTarget(gfx, pTexture, face)
	{
	}

	void OutputOnlyRenderTarget::Bind(Graphics& gfx) noexcpt
	{
		assert("Cannot bind OuputOnlyRenderTarget as shader input" && false);
	}
}
