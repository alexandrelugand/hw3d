#pragma once

namespace Bind
{
	class Rasterizer : public Bindable
	{
	public:
		Rasterizer(Graphics& gfx, CullMode cull);
		void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<Rasterizer> Resolve(Graphics& gfx, CullMode cull = CullMode::Back);
		static std::string GenerateUID(CullMode cull);
		std::string GetUID() const noexcept override;

	protected:
		ComPtr<ID3D11RasterizerState> pRasterizer;
		CullMode cull;
	};
}
