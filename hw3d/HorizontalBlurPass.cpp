#include "stdafx.h"
#include "HorizontalBlurPass.h"

namespace Rgph
{
	HorizontalBlurPass::HorizontalBlurPass(Graphics& gfx, std::string name, UINT fullWidth, UINT fullHeight)
		: FullscreenPass(gfx, std::move(name))
	{
		AddBind(Bind::PixelShader::Resolve(gfx, "BlurOutline_PS.cso"));
		AddBind(Bind::Blender::Resolve(gfx, false));
		AddBind(Bind::Sampler::Resolve(gfx, Bind::Sampler::Type::Point, true));

		AddBindSink<Bind::RenderTarget>("scratchIn");
		AddBindSink<Bind::CachingPixelCBuf>("kernel");
		RegisterSink(DirectBindableSink<Bind::CachingPixelCBuf>::Make("direction", direction));

		// the renderTarget is internally sourced and then exported as a Bindable
		renderTarget = std::make_shared<Bind::ShaderInputRenderTarget>(gfx, fullWidth / 2, fullHeight / 2, 0u);
		RegisterSource(DirectBindableSource<Bind::RenderTarget>::Make("scratchOut", renderTarget));
	}

	// this override is necessary because we cannot (yet) link input bindables directly into
	// the container of bindables (mainly because vector growth buggers references)
	void HorizontalBlurPass::Execute(Graphics& gfx) const noexcpt
	{
		auto buf = direction->GetBuffer();
		buf["isHorizontal"] = true;
		direction->SetBuffer(buf);

		direction->Bind(gfx);
		FullscreenPass::Execute(gfx);
	}
}
