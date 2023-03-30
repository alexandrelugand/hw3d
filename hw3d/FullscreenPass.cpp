#include "stdafx.h"
#include "FullscreenPass.h"

namespace Rgph
{
	FullscreenPass::FullscreenPass(Graphics& gfx, const std::string& name)
		: BindingPass(std::move(name))
	{
		// setup fullscreen geometry
		Dvtx::VertexLayout layout;
		layout.Append(Dvtx::VertexLayout::Position2D);
		Dvtx::VertexBufferDescriptor vbd(layout);
		vbd.EmplaceBack(XMFLOAT2{-1, 1});
		vbd.EmplaceBack(XMFLOAT2{1, 1});
		vbd.EmplaceBack(XMFLOAT2{-1, -1});
		vbd.EmplaceBack(XMFLOAT2{1, -1});
		AddBind(Bind::VertexBuffer::Resolve(gfx, "$Full", std::move(vbd)));
		std::vector<unsigned short> indices = {0, 1, 2, 1, 3, 2};
		AddBind(Bind::IndexBuffer::Resolve(gfx, "$Full", std::move(indices)));

		// setup other common fullscreen bindables
		auto vs = Bind::VertexShader::Resolve(gfx, "Fullscreen_VS.cso");
		AddBind(Bind::InputLayout::Resolve(gfx, layout, vs->GetBytecode()));
		AddBind(std::move(vs));
		AddBind(Bind::Topology::Resolve(gfx));
		AddBind(Bind::Rasterizer::Resolve(gfx, CullMode::Back));
	}

	void FullscreenPass::Execute(Graphics& gfx) const noexcpt
	{
		BindAll(gfx);
		gfx.DrawIndexed(6u);
	}
}
