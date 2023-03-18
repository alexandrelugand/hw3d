#pragma once

namespace Bind
{
	class TransformAllCBuf : public TransformCBuf
	{
	public:
		TransformAllCBuf(Graphics& gfx, const Draw::Drawable& parent, UINT VS_slot = 0u, UINT PS_slot = 1u);
		void Bind(Graphics& gfx) noexcept override;

	protected:
		void UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept;

	private:
		static std::unique_ptr<PixelConstantBuffer<Transforms>> pPcbuf;
	};
}