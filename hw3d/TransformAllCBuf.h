#pragma once

namespace Bind
{
	class TransformAllCBuf : public TransformCBuf
	{
	public:
		TransformAllCBuf(Graphics& gfx, UINT VS_slot = 0u, UINT PS_slot = 1u);
		void Bind(Graphics& gfx) noexcpt override;
		std::unique_ptr<CloningBindable> Clone() const noexcept override;

	protected:
		void UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcpt override;

	private:
		static std::unique_ptr<PixelConstantBuffer<Transforms>> pPcbuf;
	};
}
