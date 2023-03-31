#pragma once
#include "VertexLayout.h"

namespace Bind
{
	class InputLayout : public Bindable
	{
	public:
		InputLayout(Graphics& gfx, Dvtx::VertexLayout layout_in, const VertexShader& vs);

		Dvtx::VertexLayout GetLayout() const noexcept;
		void Bind(Graphics& gfx) noexcpt override;

		static std::shared_ptr<InputLayout> Resolve(Graphics& gfx, const Dvtx::VertexLayout& layout, const VertexShader& vs);
		static std::string GenerateUID(const Dvtx::VertexLayout& layout, const VertexShader& vs);
		std::string GetUID() const noexcept override;

	private:
		std::string vertexShaderUID;
		Dvtx::VertexLayout layout;
		ComPtr<ID3D11InputLayout> pInputLayout;
	};
}
