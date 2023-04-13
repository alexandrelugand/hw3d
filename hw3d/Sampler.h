#pragma once

namespace Bind
{
	class Sampler : public Bindable
	{
	public:
		enum class Type
		{
			Anisotropic,
			Bilinear,
			Point
		};

		Sampler(Graphics& gfx, Type type, bool reflect, unsigned int slot);
		void Bind(Graphics& gfx) noexcpt override;

		static std::shared_ptr<Sampler> Resolve(Graphics& gfx, Type type = Type::Anisotropic, bool reflect = false, unsigned int slot = 0u);
		static std::string GenerateUID(Type type, bool reflect, unsigned int slot);
		std::string GetUID() const noexcept override;

	protected:
		ComPtr<ID3D11SamplerState> pSampler;
		Type type;
		bool reflect;
		unsigned int slot;
	};
}
