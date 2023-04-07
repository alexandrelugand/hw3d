#pragma once

namespace Bind
{
	class Viewport : public Bindable
	{
	public:
		Viewport(Graphics& gfx);
		Viewport(Graphics& gfx, float width, float height);

		void Bind(Graphics& gfx) noexcpt override;

	private:
		D3D11_VIEWPORT vp{};
	};
}
