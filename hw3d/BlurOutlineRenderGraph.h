#pragma once
#include "RenderGraph.h"

namespace Rgph
{
	class BlurOutlineRenderGraph : public RenderGraph
	{
	public:
		BlurOutlineRenderGraph(Graphics& gfx);
		void RenderWidgets(Graphics& gfx);

	private:
		void SetKernelGauss(int radius, float sigma) noexcpt;
		void SetKernelBox(int radius) noexcpt;

		enum class KernelType
		{
			Gauss,
			Box,
		} kernelType = KernelType::Gauss;

		static constexpr int maxRadius = 7;
		int radius = 4;
		float sigma = 2.0f;
		std::shared_ptr<Bind::CachingPixelCBuf> blurKernel;
		std::shared_ptr<Bind::CachingPixelCBuf> blurDirection;
	};
}
