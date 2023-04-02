#include "stdafx.h"
#include "LambertianPass.h"

namespace Rgph
{
	LambertianPass::LambertianPass(Graphics& gfx, std::string name)
		: RenderQueuePass(std::move(name)),
		  pShadowSampler{std::make_shared<Bind::ShadowSampler>(gfx)},
		  pShadowCBuf{std::make_shared<Bind::ShadowCameraCBuf>(gfx)}
	{
		AddBind(pShadowCBuf);
		AddBind(pShadowSampler);
		RegisterSink(DirectBufferSink<Bind::RenderTarget>::Make("renderTarget", renderTarget));
		RegisterSink(DirectBufferSink<Bind::DepthStencil>::Make("depthStencil", depthStencil));
		AddBindSink<Bind::Bindable>("shadowMap");
		RegisterSource(DirectBufferSource<Bind::RenderTarget>::Make("renderTarget", renderTarget));
		RegisterSource(DirectBufferSource<Bind::DepthStencil>::Make("depthStencil", depthStencil));
		AddBind(Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Off));
	}

	void LambertianPass::BindMainCamera(Entities::Camera& camera) noexcept
	{
		pMainCamera = &camera;
	}

	void LambertianPass::BindShadowCamera(Entities::Camera& camera) noexcept
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
