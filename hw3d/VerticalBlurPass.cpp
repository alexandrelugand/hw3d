#include "stdafx.h"
#include "VerticalBlurPass.h"

namespace Rgph
{
	VerticalBlurPass::VerticalBlurPass(Graphics& gfx, std::string name)
		: FullscreenPass(gfx, std::move(name))
	{
		AddBind(Bind::PixelShader::Resolve(gfx, "BlurOutline_PS.cso"));
		AddBind(Bind::Blender::Resolve(gfx, true));
		AddBind(Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Mask));
		AddBind(Bind::Sampler::Resolve(gfx, Bind::Sampler::Type::Bilinear, true));

		AddBindSink<Bind::RenderTarget>("scratchIn");
		AddBindSink<Bind::CachingPixelCBuf>("kernel");

		RegisterSink(DirectBindableSink<Bind::CachingPixelCBuf>::Make("direction", direction));
		RegisterSink(DirectBufferSink<Bind::RenderTarget>::Make("renderTarget", renderTarget));
		RegisterSink(DirectBufferSink<Bind::DepthStencil>::Make("depthStencil", depthStencil));

		RegisterSource(DirectBufferSource<Bind::RenderTarget>::Make("renderTarget", renderTarget));
		RegisterSource(DirectBufferSource<Bind::DepthStencil>::Make("depthStencil", depthStencil));
	}

	void VerticalBlurPass::Execute(Graphics& gfx) const noexcpt
	{
		auto buf = direction->GetBuffer();
		buf["isHorizontal"] = false;
		direction->SetBuffer(buf);
		direction->Bind(gfx);

		FullscreenPass::Execute(gfx);
	}
}
