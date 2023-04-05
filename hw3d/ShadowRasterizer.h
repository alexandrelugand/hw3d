#pragma once

namespace Bind
{
	class ShadowRasterizer : public Bindable
	{
	public:
		ShadowRasterizer(Graphics& gfx, int depthBias, float slopeBias, float clamp);

		void Bind(Graphics& gfx) noexcpt override;

		void ChangeDepthBiasParameters(Graphics& gfx, int depthBias, float slopeBias, float clamp);
		int GetDepthBias() const;
		float GetSlopeBias() const;
		float GetClamp() const;

	protected:
		ComPtr<ID3D11RasterizerState> pRasterizer = nullptr;
		int depthBias{};
		float slopeBias{};
		float clamp{};
	};
}
