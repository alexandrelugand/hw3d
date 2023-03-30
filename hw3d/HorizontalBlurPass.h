#pragma once

namespace Rgph
{
	class HorizontalBlurPass : public FullscreenPass
	{
	public:
		HorizontalBlurPass(Graphics& gfx, std::string name, UINT fullWidth, UINT fullHeight);

		void Execute(Graphics& gfx) const noexcpt override;

	private:
		std::shared_ptr<Bind::CachingPixelCBuf> direction;
	};
}
