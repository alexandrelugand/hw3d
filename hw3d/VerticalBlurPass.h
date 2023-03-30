#pragma once

namespace Rgph
{
	class VerticalBlurPass : public FullscreenPass
	{
	public:
		VerticalBlurPass(Graphics& gfx, std::string name);

		void Execute(Graphics& gfx) const noexcpt override;

	private:
		std::shared_ptr<Bind::CachingPixelCBuf> direction;
	};
}
