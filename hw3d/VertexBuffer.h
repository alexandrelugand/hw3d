#pragma once
#include "VertexDescriptor.h"

#pragma warning(disable : 4267)

class VertexBuffer : public Bindable
{
public:
	template <class V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
		: stride(static_cast<UINT>(sizeof(V)))
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC vbd{};
		vbd.ByteWidth = sizeof(V) * vertices.size();
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0u;
		vbd.MiscFlags = 0u;
		vbd.StructureByteStride = sizeof(V);

		D3D11_SUBRESOURCE_DATA vsd{};
		vsd.pSysMem = vertices.data();

		// Create vertex buffer
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&vbd, &vsd, &pVertexBuffer));
	}

	VertexBuffer(Graphics& gfx, const VertexBufferDescriptor& descriptor)
		: stride(static_cast<UINT>(descriptor.GetDescriptor().Size()))
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC vbd{};
		vbd.ByteWidth = descriptor.Size();
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0u;
		vbd.MiscFlags = 0u;
		vbd.StructureByteStride = stride;

		D3D11_SUBRESOURCE_DATA vsd{};
		vsd.pSysMem = descriptor.GetData();

		// Create vertex buffer
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&vbd, &vsd, &pVertexBuffer));
	}

	void Bind(Graphics& gfx) noexcpt override;

private:
	UINT stride;
	ComPtr<ID3D11Buffer> pVertexBuffer;
};

#pragma warning(default : 4267)
