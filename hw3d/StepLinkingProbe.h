#pragma once

namespace Probes
{
	class StepLinkingProbe : public TechniqueProbe
	{
	public:
		StepLinkingProbe(Rgph::RenderGraph& rg)
			: rg(rg)
		{
		}

	protected:
		void OnSetStep() override;

	private:
		Rgph::RenderGraph& rg;
	};
}
