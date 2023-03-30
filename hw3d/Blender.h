#pragma once

namespace Bind
{
	class Blender : public Bindable
	{
	public:
		Blender(Graphics& gfx, bool blending, std::optional<float> factor = {});
		void Bind(Graphics& gfx) noexcpt override;

		void SetFactor(float factor) noexcpt;
		float GetFactor() const noexcpt;

		static std::shared_ptr<Blender> Resolve(Graphics& gfx, bool blending, std::optional<float> factor = {});
		static std::string GenerateUID(bool blending, std::optional<float> factor);
		std::string GetUID() const noexcept override;

	protected:
		ComPtr<ID3D11BlendState> pBlender;
		bool blending;
		std::optional<std::array<float, 4>> factors;
	};
}
