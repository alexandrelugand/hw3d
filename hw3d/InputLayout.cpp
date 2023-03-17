#include "stdafx.h"
#include "InputLayout.h"

namespace Bind
{
	InputLayout::InputLayout(Graphics& gfx, Dvtx::VertexLayout layout_in, ID3DBlob* pVertexShaderBytecode)
		: layout(std::move(layout_in))
	{
		INFOMAN(gfx);

		const auto d3dlayout = layout.GetD3DLayout();

		GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(d3dlayout.data(),
				static_cast<UINT>(d3dlayout.size()),
				pVertexShaderBytecode->GetBufferPointer(),
				pVertexShaderBytecode->GetBufferSize(),
				&pInputLayout)
		);
	}

	void InputLayout::Bind(Graphics& gfx) noexcpt
	{
		GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
	}

	std::shared_ptr<InputLayout> InputLayout::Resolve(Graphics& gfx, const Dvtx::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode)
	{
		return Codex::Resolve<InputLayout>(gfx, layout, pVertexShaderBytecode);
	}

	std::string InputLayout::GenerateUID(const Dvtx::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode)
	{
		return typeid(InputLayout).name() + "#"s + layout.GetCode();
	}

	std::string InputLayout::GetUID() const noexcept
	{
		return GenerateUID(layout);
	}
}
