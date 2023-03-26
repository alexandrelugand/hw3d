#include "stdafx.h"
#include "Sampler.h"

namespace Bind
{
	Sampler::Sampler(Graphics& gfx, bool anisotropic, bool reflect)
		: anisotropic(anisotropic), reflect(reflect)
	{
		INFOMAN(gfx);

		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{CD3D11_DEFAULT{}};
		samplerDesc.Filter = anisotropic ? D3D11_FILTER_ANISOTROPIC : D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

		GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler));
	}

	void Sampler::Bind(Graphics& gfx) noexcpt
	{
		GetContext(gfx)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
	}

	std::shared_ptr<Sampler> Sampler::Resolve(Graphics& gfx, bool anisotropic, bool reflect)
	{
		return Codex::Resolve<Sampler>(gfx, anisotropic, reflect);
	}

	std::string Sampler::GenerateUID(bool anisotropic, bool reflect)
	{
		return typeid(Sampler).name() + "#"s + (anisotropic ? "A"s : "a"s) + (reflect ? "R"s : "W"s);
	}

	std::string Sampler::GetUID() const noexcept
	{
		return GenerateUID(anisotropic, reflect);
	}
}
