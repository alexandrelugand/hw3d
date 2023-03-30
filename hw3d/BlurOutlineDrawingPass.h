#pragma once

namespace Rgph
{
	class BlurOutlineDrawingPass : public RenderQueuePass
	{
	public:
		BlurOutlineDrawingPass(Graphics& gfx, std::string name, UINT fullWidth, UINT fullHeight);

		void Execute(Graphics& gfx) const noexcpt override;
	};
}
