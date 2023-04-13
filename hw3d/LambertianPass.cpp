#include "stdafx.h"
#include "LambertianPass.h"

namespace Rgph
{
	LambertianPass::LambertianPass(Graphics& gfx, std::string name)
		: RenderQueuePass(std::move(name)),
		  pShadowCBuf{std::make_shared<Bind::ShadowCameraCBuf>(gfx, Shaders::CBuf::Shadow)}
	{
		AddBind(pShadowCBuf);
		RegisterSink(DirectBufferSink<Bind::RenderTarget>::Make("renderTarget", renderTarget));
		RegisterSink(DirectBufferSink<Bind::DepthStencil>::Make("depthStencil", depthStencil));
		AddBindSink<Bind::Bindable>("shadowMap");
		AddBind(std::make_shared<Bind::ShadowSampler>(gfx));
		AddBind(std::make_shared<Bind::Sampler>(gfx, Bind::Sampler::Type::Anisotropic, false, 2));
		RegisterSource(DirectBufferSource<Bind::RenderTarget>::Make("renderTarget", renderTarget));
		RegisterSource(DirectBufferSource<Bind::DepthStencil>::Make("depthStencil", depthStencil));
		AddBind(Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Off));
		AddBind(Bind::Rasterizer::Resolve(gfx, CullMode::Back));
	}

	void LambertianPass::BindMainCamera(Entities::Camera& camera) noexcept
	{
		pMainCamera = &camera;
	}

	void LambertianPass::BindShadowCamera(Entities::Camera& camera) const noexcept
	{
		pShadowCBuf->SetCamera(&camera);
	}

	void LambertianPass::Execute(Graphics& gfx) const noexcpt
	{
		assert(pMainCamera);
		pShadowCBuf->Update(gfx);
		pMainCamera->BindToGraphics(gfx);
		RenderQueuePass::Execute(gfx);
	}
}
