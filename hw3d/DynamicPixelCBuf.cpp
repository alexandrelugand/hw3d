#include "stdafx.h"
#include "DynamicPixelCBuf.h"

namespace Bind
{
	void DynamicPixelCBuf::Update(Graphics& gfx, const Dcb::Buffer& buf)
	{
		assert(&buf.GetRootLayoutElement() == &GetRootLayoutElement());
		INFOMAN(gfx);

		D3D11_MAPPED_SUBRESOURCE msr;
		GFX_THROW_INFO(GetContext(gfx)->Map(
			pConstantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr
		));
		memcpy(msr.pData, buf.GetData(), buf.GetSizeInBytes());
		GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
	}

	void DynamicPixelCBuf::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}

	DynamicPixelCBuf::DynamicPixelCBuf(Graphics& gfx, const Dcb::LayoutElement& layoutRoot, UINT slot, const Dcb::Buffer* pBuf)
		: slot(slot)
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = static_cast<UINT>(layoutRoot.GetSizeInBytes());
		cbd.StructureByteStride = 0u;

		if (pBuf != nullptr)
		{
			D3D11_SUBRESOURCE_DATA csd = {};
			csd.pSysMem = pBuf->GetData();
			GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer));
		}
		else
		{
			GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
		}
	}

	CachingDynamicPixelCBuf::CachingDynamicPixelCBuf(Graphics& gfx, const Dcb::CookedLayout& layout, UINT slot)
		: DynamicPixelCBuf(gfx, *layout.ShareRoot(), slot, nullptr),
		  buf(Dcb::Buffer(layout))
	{
	}

	CachingDynamicPixelCBuf::CachingDynamicPixelCBuf(Graphics& gfx, const Dcb::Buffer& buf, UINT slot)
		: DynamicPixelCBuf(gfx, buf.GetRootLayoutElement(), slot, &buf),
		  buf(buf)
	{
	}

	const Dcb::LayoutElement& CachingDynamicPixelCBuf::GetRootLayoutElement() const noexcept
	{
		return buf.GetRootLayoutElement();
	}

	const Dcb::Buffer& CachingDynamicPixelCBuf::GetBuffer() const noexcept
	{
		return buf;
	}

	void CachingDynamicPixelCBuf::SetBuffer(const Dcb::Buffer& buf_in)
	{
		buf.CopyFrom(buf_in);
		dirty = true;
	}

	void CachingDynamicPixelCBuf::Bind(Graphics& gfx) noexcept
	{
		if (dirty)
		{
			Update(gfx, buf);
			dirty = false;
		}
		DynamicPixelCBuf::Bind(gfx);
	}

	void CachingDynamicPixelCBuf::Accept(TechniqueProbe& probe)
	{
		if (probe.VisitBuffer(buf))
		{
			dirty = true;
		}
	}

	NocacheDynamicPixelCBuf::NocacheDynamicPixelCBuf(Graphics& gfx, const Dcb::CookedLayout& layout, UINT slot)
		: DynamicPixelCBuf(gfx, *layout.ShareRoot(), slot, nullptr),
		  pLayoutRoot(layout.ShareRoot())
	{
	}

	NocacheDynamicPixelCBuf::NocacheDynamicPixelCBuf(Graphics& gfx, const Dcb::Buffer& buf, UINT slot)
		: DynamicPixelCBuf(gfx, buf.GetRootLayoutElement(), slot, &buf),
		  pLayoutRoot(buf.ShareLayoutRoot())
	{
	}

	const Dcb::LayoutElement& NocacheDynamicPixelCBuf::GetRootLayoutElement() const noexcept
	{
		return *pLayoutRoot;
	}
}
