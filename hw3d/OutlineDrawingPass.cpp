#include "stdafx.h"
#include "OutlineDrawingPass.h"

namespace Rgph
{
	OutlineDrawingPass::OutlineDrawingPass(Graphics& gfx, std::string name)
		: RenderQueuePass(std::move(name))
	{
		RegisterSink(DirectBufferSink<Bind::RenderTarget>::Make("renderTarget", renderTarget));
		RegisterSink(DirectBufferSink<Bind::DepthStencil>::Make("depthStencil", depthStencil));
		RegisterSource(DirectBufferSource<Bind::RenderTarget>::Make("renderTarget", renderTarget));
		RegisterSource(DirectBufferSource<Bind::DepthStencil>::Make("depthStencil", depthStencil));
		AddBind(Bind::VertexShader::Resolve(gfx, "Solid_VS.cso"));
		AddBind(Bind::PixelShader::Resolve(gfx, "Solid_PS.cso"));
		AddBind(Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Mask));
		AddBind(Bind::Rasterizer::Resolve(gfx, CullMode::Back));
	}
}
