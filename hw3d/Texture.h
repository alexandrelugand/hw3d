#pragma once
#include "Forwards.h"

namespace Bind
{
	class Texture : public Bindable
	{
	public:
		Texture(Graphics& gfx, const Draw::Surface& s);
		void Bind(Graphics& gfx) noexcpt override;

	protected:
		ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}
