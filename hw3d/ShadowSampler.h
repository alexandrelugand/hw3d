#pragma once

namespace Bind
{
	class ShadowSampler : public Bindable
	{
	public:
		ShadowSampler(Graphics& gfx);
		void Bind(Graphics& gfx) noexcpt override;

	protected:
		ComPtr<ID3D11SamplerState> pSampler;
	};
}
