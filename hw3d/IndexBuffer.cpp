#include "stdafx.h"
#include "IndexBuffer.h"

namespace Bind
{
	IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices)
		: IndexBuffer(gfx, "?", indices)
	{
	}

	IndexBuffer::IndexBuffer(Graphics& gfx, std::string tag, const std::vector<unsigned short>& indices)
		: tag(tag), count(static_cast<UINT>(indices.size()))
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC ibd;
		ibd.ByteWidth = count * sizeof(unsigned short);
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.StructureByteStride = sizeof(unsigned short);

		D3D11_SUBRESOURCE_DATA isd{};
		isd.pSysMem = indices.data();

		// Create index buffer
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&ibd, &isd, &pIndexBuffer));
	}

	void IndexBuffer::Bind(Graphics& gfx) noexcpt
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u));
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Resolve(Graphics& gfx, const std::string& tag, const std::vector<unsigned short>& indices)
	{
		assert(tag != "?"); //Don't resolve from codex if no tag is defined
		return Codex::Resolve<IndexBuffer>(gfx, tag, indices);
	}

	std::string IndexBuffer::GetUID() const noexcept
	{
		return GenerateUID_(tag);
	}

	std::string IndexBuffer::GenerateUID_(const std::string& tag)
	{
		return typeid(IndexBuffer).name() + "#"s + tag;
	}
}
