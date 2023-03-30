#include "stdafx.h"
#include "VertexShader.h"

namespace Bind
{
	VertexShader::VertexShader(Graphics& gfx, const std::string& path)
		: path(path)
	{
		INFOMAN(gfx);

		GFX_THROW_INFO(D3DReadFileToBlob(String::ToWide(path).c_str(), &pBlob));
		GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
	}

	void VertexShader::Bind(Graphics& gfx) noexcpt
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u));
	}

	ID3DBlob* VertexShader::GetBytecode() const noexcept
	{
		return pBlob.Get();
	}

	std::shared_ptr<VertexShader> VertexShader::Resolve(Graphics& gfx, const std::string& path)
	{
		return Codex::Resolve<VertexShader>(gfx, path);
	}

	std::string VertexShader::GenerateUID(const std::string& path)
	{
		return typeid(VertexShader).name() + "#"s + path;
	}

	std::string VertexShader::GetUID() const noexcept
	{
		return GenerateUID(path);
	}
}
