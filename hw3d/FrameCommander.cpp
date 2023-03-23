#include "stdafx.h"
#include "FrameCommander.h"

void FrameCommander::Accept(Job& job, size_t targetPass)
{
	passes[targetPass].Accept(job);
}

void FrameCommander::Execute(Graphics& gfx) const noexcept(!true)
{
	// normally this would be a loop with each pass defining it setup / etc.
	// and later on it would be a complex graph with parallel execution contingent
	// on input / output requirements

	// main phong lighting pass
	Bind::Rasterizer::Resolve(gfx, Back)->Bind(gfx);
	Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Off)->Bind(gfx);
	passes[0].Execute(gfx);

	// outline masking pass
	Bind::Rasterizer::Resolve(gfx, Back)->Bind(gfx);
	Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Write)->Bind(gfx);
	Bind::NullPixelShader::Resolve(gfx)->Bind(gfx);
	passes[1].Execute(gfx);

	// outline drawing pass
	Bind::Rasterizer::Resolve(gfx, Back)->Bind(gfx);
	Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Mask)->Bind(gfx);
	passes[2].Execute(gfx);
}

void FrameCommander::Reset() noexcept
{
	for (auto& p : passes)
	{
		p.Reset();
	}
}
