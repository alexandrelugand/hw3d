#pragma once
#include "RenderGraph.h"

namespace Rgph
{
	class BlurOutlineRenderGraph : public RenderGraph
	{
	public:
		BlurOutlineRenderGraph(Graphics& gfx);

	private:
		void SetKernelGauss(int radius, float sigma) noexcpt;
		static constexpr int maxRadius = 7;
		static constexpr int radius = 4;
		static constexpr float sigma = 2.0f;

		std::shared_ptr<Bind::CachingPixelCBuf> blurKernel;
		std::shared_ptr<Bind::CachingPixelCBuf> blurDirection;
	};
}
