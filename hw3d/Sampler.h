#pragma once

namespace Bind
{
	class Sampler : public Bindable
	{
	public:
		Sampler(Graphics& gfx, bool anisotropic, bool reflect);
		void Bind(Graphics& gfx) noexcpt override;

		static std::shared_ptr<Sampler> Resolve(Graphics& gfx, bool anisotropic = true, bool reflect = false);
		static std::string GenerateUID(bool anisotropic, bool reflect);
		std::string GetUID() const noexcept override;

	protected:
		ComPtr<ID3D11SamplerState> pSampler;
		bool anisotropic;
		bool reflect;
	};
}
