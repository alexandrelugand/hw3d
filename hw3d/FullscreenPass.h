#pragma once

namespace Rgph
{
	class FullscreenPass : public BindingPass
	{
	public:
		FullscreenPass(Graphics& gfx, const std::string& name);

		void Execute(Graphics& gfx) const noexcpt override;
	};
}
