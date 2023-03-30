#include "stdafx.h"
#include "BlurOutlineDrawingPass.h"

namespace Rgph
{
	BlurOutlineDrawingPass::BlurOutlineDrawingPass(Graphics& gfx, std::string name, UINT fullWidth, UINT fullHeight)
		: RenderQueuePass(std::move(name))
	{
		renderTarget = std::make_unique<Bind::ShaderInputRenderTarget>(gfx, fullWidth / 2, fullHeight / 2, 0);
		AddBind(Bind::VertexShader::Resolve(gfx, "Solid_VS.cso"));
		AddBind(Bind::PixelShader::Resolve(gfx, "Solid_PS.cso"));
		AddBind(Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Mask));
		AddBind(Bind::Blender::Resolve(gfx, false));
		RegisterSource(DirectBindableSource<Bind::RenderTarget>::Make("scratchOut", renderTarget));
	}

	void BlurOutlineDrawingPass::Execute(Graphics& gfx) const noexcpt
	{
		renderTarget->Clear(gfx);
		RenderQueuePass::Execute(gfx);
	}
}
