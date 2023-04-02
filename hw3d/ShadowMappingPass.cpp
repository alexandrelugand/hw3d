#include "stdafx.h"
#include "ShadowMappingPass.h"

namespace Rgph
{
	ShadowMappingPass::ShadowMappingPass(Graphics& gfx, std::string name)
		: RenderQueuePass(std::move(name))
	{
		depthStencil = std::make_unique<Bind::ShaderInputDepthStencil>(gfx, 3, Bind::DepthStencil::Usage::ShadowDepth);
		AddBind(Bind::VertexShader::Resolve(gfx, "Solid_VS.cso"));
		AddBind(Bind::NullPixelShader::Resolve(gfx));
		AddBind(Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Off));
		AddBind(Bind::Blender::Resolve(gfx, false));
		RegisterSource(DirectBindableSource<Bind::DepthStencil>::Make("map", depthStencil));
	}

	void ShadowMappingPass::BindShadowCamera(Entities::Camera& camera)
	{
		pShadowCamera = &camera;
	}

	void ShadowMappingPass::Execute(Graphics& gfx) const noexcept(!true)
	{
		assert(pShadowCamera);
		depthStencil->Clear(gfx);
		pShadowCamera->BindToGraphics(gfx);
		RenderQueuePass::Execute(gfx);
	}

	void ShadowMappingPass::DumpShadowMap(Graphics& gfx, const std::string& path) const
	{
		depthStencil->ToSurface(gfx).Save(path);
	}
}
