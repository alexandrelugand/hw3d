#pragma once
#include "VertexLayout.h"

namespace Bind
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(Graphics& gfx, Dvtx::VertexLayout layout_in, ID3DBlob* pVertexShaderBytecode);

		Dvtx::VertexLayout GetLayout() const noexcept;
		void Bind(Graphics& gfx) noexcpt override;

		static std::shared_ptr<InputLayout> Resolve(Graphics& gfx, const Dvtx::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode);
		static std::string GenerateUID(const Dvtx::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode = nullptr);
		std::string GetUID() const noexcept override;

	private:
		Dvtx::VertexLayout layout;
		ComPtr<ID3D11InputLayout> pInputLayout;
	};
}
