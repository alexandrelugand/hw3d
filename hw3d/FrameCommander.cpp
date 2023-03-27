#include "stdafx.h"
#include "FrameCommander.h"

FrameCommander::FrameCommander(Graphics& gfx)
	: ds(gfx, gfx.GetWidth(), gfx.GetHeight()),
	  rt1({gfx, gfx.GetWidth() / downFactor, gfx.GetHeight() / downFactor}),
	  rt2({gfx, gfx.GetWidth() / downFactor, gfx.GetHeight() / downFactor}),
	  blur(gfx, 7, 2.6f, "BlurOutline_PS.cso")
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
	pVsFull = Bind::VertexShader::Resolve(gfx, "Fullscreen_VS.cso");
	pLayoutFull = Bind::InputLayout::Resolve(gfx, lay, pVsFull->GetBytecode());
	pSamplerFullPoint = Bind::Sampler::Resolve(gfx, false, true);
	pSamplerFullAnistropic = Bind::Sampler::Resolve(gfx, true, true);
	pBlenderMerge = Bind::Blender::Resolve(gfx, true);
}

void FrameCommander::Accept(Job& job, size_t targetPass)
{
	passes[targetPass].Accept(job);
}

void FrameCommander::Execute(Graphics& gfx) noexcpt
{
	// normally this would be a loop with each pass defining it setup / etc.
	// and later on it would be a complex graph with parallel execution contingent
	// on input / output requirements

	// setup render target used for normal passes
	ds.Clear(gfx);
	rt1->Clear(gfx);
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
	rt1->BindAsTarget(gfx);
	Bind::Rasterizer::Resolve(gfx, Back)->Bind(gfx);
	Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Off)->Bind(gfx);
	passes[2].Execute(gfx);

	// fullscreen blur h-pass
	rt2->BindAsTarget(gfx);
	rt1->BindAsTexture(gfx, 0);
	pVbFull->Bind(gfx);
	pIbFull->Bind(gfx);
	pVsFull->Bind(gfx);
	pLayoutFull->Bind(gfx);
	pSamplerFullPoint->Bind(gfx);
	blur.Bind(gfx);
	blur.SetHorizontal(gfx);
	gfx.DrawIndexed(pIbFull->GetCount());

	// fullscreen blur v-pass + combine
	gfx.BindSwapBuffer(ds);
	rt2->BindAsTexture(gfx, 0u);
	pBlenderMerge->Bind(gfx);
	pSamplerFullAnistropic->Bind(gfx);
	Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Mask)->Bind(gfx);
	blur.SetVertical(gfx);

	gfx.DrawIndexed(pIbFull->GetCount());
}

void FrameCommander::Reset() noexcept
{
	for (auto& p : passes)
	{
		p.Reset();
	}
}

void FrameCommander::ShowWindows(Graphics& gfx)
{
	if (ImGui::Begin("Blur"))
	{
		if (ImGui::SliderInt("Down Factor", &downFactor, 1, 16))
		{
			rt1.emplace(gfx, gfx.GetWidth() / downFactor, gfx.GetHeight() / downFactor);
			rt2.emplace(gfx, gfx.GetWidth() / downFactor, gfx.GetHeight() / downFactor);
		}
		blur.RenderWidgets(gfx);
	}
	ImGui::End();
}
