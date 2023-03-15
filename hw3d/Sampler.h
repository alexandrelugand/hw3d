#pragma once

namespace Bind
{
	class Sampler : public Bindable
	{
	public:
		Sampler(Graphics& gfx);
		void Bind(Graphics& gfx) noexcpt override;

	protected:
		ComPtr<ID3D11SamplerState> pSampler;
	};
}
