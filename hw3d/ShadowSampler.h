#pragma once

namespace Bind
{
	class ShadowSampler : public Bindable
	{
	public:
		ShadowSampler(Graphics& gfx);
		void Bind(Graphics& gfx) noexcpt override;

		void SetBilinear(bool bilin);
		void SetHwPcf(bool hwPcf);
		bool GetBilinear() const;
		bool GetHwPcf() const;

	protected:
		size_t curSampler;
		ComPtr<ID3D11SamplerState> samplers[4];

	private:
		unsigned int GetCurrentSlot() const;
		static size_t ShadowSamplerIndex(bool bilin, bool hwPcf);
		static ComPtr<ID3D11SamplerState> MakeSampler(Graphics& gfx, bool bilin, bool hwPcf);
	};
}
