#pragma once

namespace Bind
{
	class TransformAllCBuf : public TransformCBuf
	{
	public:
		TransformAllCBuf(Graphics& gfx, unsigned int VS_slot, unsigned int PS_slot);
		void Bind(Graphics& gfx) noexcpt override;
		std::unique_ptr<CloningBindable> Clone() const noexcept override;

	protected:
		void UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcpt override;

	private:
		static std::unique_ptr<PixelConstantBuffer<Transforms>> pPcbuf;
	};
}
