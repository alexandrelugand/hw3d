#pragma once
#include "DynamicConstant.h"

namespace Bind
{
	class DynamicCBuf : public Bindable
	{
	public:
		void Update(Graphics& gfx, const Dcb::Buffer& buf);
		virtual const Dcb::LayoutElement& GetRootLayoutElement() const noexcept = 0;

	protected:
		DynamicCBuf(Graphics& gfx, const Dcb::LayoutElement& layoutRoot, UINT slot, const Dcb::Buffer* pBuf);

		ComPtr<ID3D11Buffer> pConstantBuffer;
		UINT slot;
	};

	class DynamicPixelCBuf : public DynamicCBuf
	{
	public:
		using DynamicCBuf::DynamicCBuf;
		void Bind(Graphics& gfx) noexcept override;
	};

	class DynamicVertexCBuf : public DynamicCBuf
	{
	public:
		using DynamicCBuf::DynamicCBuf;
		void Bind(Graphics& gfx) noexcept override;
	};

	template <class T>
	class CachingDynamicCBuf : public T
	{
	public:
		CachingDynamicCBuf(Graphics& gfx, const Dcb::CookedLayout& layout, UINT slot)
			: T(gfx, *layout.ShareRoot(), slot, nullptr),
			  buf(Dcb::Buffer(layout))
		{
		}

		CachingDynamicCBuf(Graphics& gfx, const Dcb::Buffer& buf, UINT slot)
			: T(gfx, buf.GetRootLayoutElement(), slot, &buf),
			  buf(buf)
		{
		}

		const Dcb::LayoutElement& GetRootLayoutElement() const noexcept override
		{
			return buf.GetRootLayoutElement();
		}

		const Dcb::Buffer& GetBuffer() const noexcept
		{
			return buf;
		}

		void SetBuffer(const Dcb::Buffer& buf_in)
		{
			buf.CopyFrom(buf_in);
			dirty = true;
		}

		void Bind(Graphics& gfx) noexcept override
		{
			if (dirty)
			{
				T::Update(gfx, buf);
				dirty = false;
			}
			T::Bind(gfx);
		}

		void Accept(TechniqueProbe& probe) override
		{
			if (probe.VisitBuffer(buf))
			{
				dirty = true;
			}
		}

	private:
		bool dirty = false;
		Dcb::Buffer buf;
	};

	using CachingPixelCBuf = CachingDynamicCBuf<DynamicPixelCBuf>;
	using CachingVertexCBuf = CachingDynamicCBuf<DynamicVertexCBuf>;
}
