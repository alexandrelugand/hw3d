#pragma once
#include "DynamicConstant.h"

namespace Bind
{
	class DynamicPixelCBuf : public Bindable
	{
	public:
		void Update(Graphics& gfx, const Dcb::Buffer& buf);
		void Bind(Graphics& gfx) noexcept override;

		virtual const Dcb::LayoutElement& GetRootLayoutElement() const noexcept = 0;

	protected:
		DynamicPixelCBuf(Graphics& gfx, const Dcb::LayoutElement& layoutRoot, UINT slot, const Dcb::Buffer* pBuf);

	private:
		ComPtr<ID3D11Buffer> pConstantBuffer;
		UINT slot;
	};

	class CachingDynamicPixelCBuf : public DynamicPixelCBuf
	{
	public:
		CachingDynamicPixelCBuf(Graphics& gfx, const Dcb::CookedLayout& layout, UINT slot);
		CachingDynamicPixelCBuf(Graphics& gfx, const Dcb::Buffer& buf, UINT slot);

		const Dcb::LayoutElement& GetRootLayoutElement() const noexcept override;

		const Dcb::Buffer& GetBuffer() const noexcept;
		void SetBuffer(const Dcb::Buffer& buf_in);

		void Bind(Graphics& gfx) noexcept override;
		void Accept(TechniqueProbe& probe) override;

	private:
		bool dirty = false;
		Dcb::Buffer buf;
	};

	class NocacheDynamicPixelCBuf : public DynamicPixelCBuf
	{
	public:
		NocacheDynamicPixelCBuf(Graphics& gfx, const Dcb::CookedLayout& layout, UINT slot);
		NocacheDynamicPixelCBuf(Graphics& gfx, const Dcb::Buffer& buf, UINT slot);

		const Dcb::LayoutElement& GetRootLayoutElement() const noexcept override;

	private:
		std::shared_ptr<Dcb::LayoutElement> pLayoutRoot;
	};
}
