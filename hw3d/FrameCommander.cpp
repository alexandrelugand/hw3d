#include "stdafx.h"
#include "FrameCommander.h"

FrameCommander::FrameCommander(Graphics& gfx)
	: ds(gfx, gfx.GetWidth(), gfx.GetHeight()),
	  rt(gfx, gfx.GetWidth(), gfx.GetHeight())
{
	// setup fullscreen geometry
	Dvtx::VertexLayout lay;
	lay.Append(Dvtx::VertexLayout::Position2D);
	Dvtx::VertexBufferDescriptor bufFull{lay};

	//Fx full render view is composed by 2 triangles
	bufFull.EmplaceBack(XMFLOAT2{-1, 1});
	bufFull.EmplaceBack(XMFLOAT2{1, 1});
	bufFull.EmplaceBack(XMFLOAT2{-1, -1});
	bufFull.EmplaceBack(XMFLOAT2{1, -1});
	pVbFull = Bind::VertexBuffer::Resolve(gfx, "$Full", std::move(bufFull));

	std::vector<unsigned short> indices = {0, 1, 2, 1, 3, 2};
	pIbFull = Bind::IndexBuffer::Resolve(gfx, "$Full", std::move(indices));

	// setup fullscreen shaders
	pPsFull = Bind::PixelShader::Resolve(gfx, "Blur_PS.cso");
	pVsFull = Bind::VertexShader::Resolve(gfx, "Fullscreen_VS.cso");
	pLayoutFull = Bind::InputLayout::Resolve(gfx, lay, pVsFull->GetBytecode());
	pSamplerFull = Bind::Sampler::Resolve(gfx, false, true);
	pBlenderFull = Bind::Blender::Resolve(gfx, true);
}

void FrameCommander::Accept(Job& job, size_t targetPass)
{
	passes[targetPass].Accept(job);
}

void FrameCommander::Execute(Graphics& gfx) const noexcept(!true)
{
	// normally this would be a loop with each pass defining it setup / etc.
	// and later on it would be a complex graph with parallel execution contingent
	// on input / output requirements

	// setup render target used for normal passes
	ds.Clear(gfx);
	rt.Clear(gfx);
	gfx.BindSwapBuffer(ds);

	// main phong lighting pass
	Bind::Blender::Resolve(gfx, false)->Bind(gfx);
	Bind::Rasterizer::Resolve(gfx, Back)->Bind(gfx);
	Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Off)->Bind(gfx);
	passes[0].Execute(gfx);

	// outline masking pass
	Bind::Rasterizer::Resolve(gfx, Back)->Bind(gfx);
	Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Write)->Bind(gfx);
	Bind::NullPixelShader::Resolve(gfx)->Bind(gfx);
	passes[1].Execute(gfx);

	// outline drawing pass
	rt.BindAsTarget(gfx);
	Bind::Rasterizer::Resolve(gfx, Back)->Bind(gfx);
	Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Off)->Bind(gfx);
	passes[2].Execute(gfx);

	// fullscreen blur + blend pass
	gfx.BindSwapBuffer(ds);
	rt.BindAsTexture(gfx, 0);
	pVbFull->Bind(gfx);
	pIbFull->Bind(gfx);
	pVsFull->Bind(gfx);
	pPsFull->Bind(gfx);
	pLayoutFull->Bind(gfx);
	pSamplerFull->Bind(gfx);
	pBlenderFull->Bind(gfx);
	Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Mask)->Bind(gfx);
	gfx.DrawIndexed(pIbFull->GetCount());
}

void FrameCommander::Reset() noexcept
{
	for (auto& p : passes)
	{
		p.Reset();
	}
}
