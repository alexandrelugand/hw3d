#include "stdafx.h"
#include "InputLayout.h"

namespace Bind
{
	InputLayout::InputLayout(Graphics& gfx, Dvtx::VertexLayout layout_in, const VertexShader& vs)
		: layout(std::move(layout_in))
	{
		INFOMAN(gfx);

		vertexShaderUID = vs.GetUID();
		const auto d3dlayout = layout.GetD3DLayout();
		const auto pByteCode = vs.GetBytecode();

		GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(d3dlayout.data(),
				static_cast<UINT>(d3dlayout.size()),
				pByteCode->GetBufferPointer(),
				pByteCode->GetBufferSize(),
				&pInputLayout)
		);
	}

	Dvtx::VertexLayout InputLayout::GetLayout() const noexcept
	{
		return layout;
	}

	void InputLayout::Bind(Graphics& gfx) noexcpt
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetInputLayout(pInputLayout.Get()));
	}

	std::shared_ptr<InputLayout> InputLayout::Resolve(Graphics& gfx, const Dvtx::VertexLayout& layout, const VertexShader& vs)
	{
		return Codex::Resolve<InputLayout>(gfx, layout, vs);
	}

	std::string InputLayout::GenerateUID(const Dvtx::VertexLayout& layout, const VertexShader& vs)
	{
		return typeid(InputLayout).name() + "#"s + layout.GetCode() + "#"s + vs.GetUID();
	}

	std::string InputLayout::GetUID() const noexcept
	{
		return typeid(InputLayout).name() + "#"s + layout.GetCode() + "#"s + vertexShaderUID;
	}
}
