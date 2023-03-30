#include "stdafx.h"
#include "OutlineMaskGenerationPass.h"

namespace Rgph
{
	OutlineMaskGenerationPass::OutlineMaskGenerationPass(Graphics& gfx, std::string name)
		: RenderQueuePass(std::move(name))
	{
		RegisterSink(DirectBufferSink<Bind::DepthStencil>::Make("depthStencil", depthStencil));
		RegisterSource(DirectBufferSource<Bind::DepthStencil>::Make("depthStencil", depthStencil));
		AddBind(Bind::VertexShader::Resolve(gfx, "Solid_VS.cso"));
		AddBind(Bind::NullPixelShader::Resolve(gfx));
		AddBind(Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Write));
		AddBind(Bind::Rasterizer::Resolve(gfx, CullMode::Back));
	}
}
