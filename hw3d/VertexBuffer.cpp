#include "stdafx.h"
#include "VertexBuffer.h"

namespace Bind
{
	VertexBuffer::VertexBuffer(Graphics& gfx, const Dvtx::VertexBufferDescriptor& vbd)
		: VertexBuffer(gfx, "?", vbd)
	{
	}

	VertexBuffer::VertexBuffer(Graphics& gfx, const std::string& tag, const Dvtx::VertexBufferDescriptor& vbd)
		: tag(tag), stride(static_cast<UINT>(vbd.GetLayout().Size()))
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC bd;
		bd.ByteWidth = static_cast<UINT>(vbd.Size());
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = stride;

		D3D11_SUBRESOURCE_DATA vsd{};
		vsd.pSysMem = vbd.GetData();

		// Create vertex buffer
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &vsd, &pVertexBuffer));
	}

	void VertexBuffer::Bind(Graphics& gfx) noexcpt
	{
		const UINT offset = 0u;
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset));
	}

	const Dvtx::VertexLayout& VertexBuffer::GetLayout() const noexcept
	{
		return layout;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Resolve(Graphics& gfx, const std::string& tag, const Dvtx::VertexBufferDescriptor& vbd)
	{
		assert(tag != "?");
		return Codex::Resolve<VertexBuffer>(gfx, tag, vbd);
	}

	std::string VertexBuffer::GetUID() const noexcept
	{
		return GenerateUID(tag);
	}

	std::string VertexBuffer::GenerateUID_(const std::string& tag)
	{
		return typeid(VertexBuffer).name() + "#"s + tag;
	}
}
