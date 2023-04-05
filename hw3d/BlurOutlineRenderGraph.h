#pragma once
#include "RenderGraph.h"

namespace Rgph
{
	class BlurOutlineRenderGraph : public RenderGraph
	{
	public:
		BlurOutlineRenderGraph(Graphics& gfx);
		void RenderWindows(Graphics& gfx);
		void BindMainCamera(Entities::Camera& camera);
		void BindShadowCamera(Entities::Camera& camera);

		void DumpShadowMap(Graphics& gfx, const std::string& path);

	private:
		void RenderShadowWindow(Graphics& gfx);
		void RenderKernelWindow(Graphics& gfx);
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
		std::shared_ptr<Bind::CachingPixelCBuf> shadowControl;
		std::shared_ptr<Bind::ShadowSampler> shadowSampler;
		std::shared_ptr<Bind::ShadowRasterizer> shadowRasterizer;
	};
}
