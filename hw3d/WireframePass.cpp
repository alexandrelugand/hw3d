#include "stdafx.h"
#include "WireframePass.h"

namespace Rgph
{
	WireframePass::WireframePass(Graphics& gfx, std::string name)
		: RenderQueuePass(std::move(name))
	{
		RegisterSink(DirectBufferSink<Bind::RenderTarget>::Make("renderTarget", renderTarget));
		RegisterSink(DirectBufferSink<Bind::DepthStencil>::Make("depthStencil", depthStencil));
		RegisterSource(DirectBufferSource<Bind::RenderTarget>::Make("renderTarget", renderTarget));
		RegisterSource(DirectBufferSource<Bind::DepthStencil>::Make("depthStencil", depthStencil));
		AddBind(Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::DepthReversed));
	}
}
