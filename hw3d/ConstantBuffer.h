#pragma once

template <typename C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(Graphics& gfx, UINT slot = 0u): slot(slot)
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC cbd{};
		cbd.ByteWidth = sizeof(C);
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.StructureByteStride = 0u;

		// Create constant buffer
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
	}

	ConstantBuffer(Graphics& gfx, const C& constant, UINT slot = 0u): slot(slot)
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC cbd{};
		cbd.ByteWidth = sizeof(constant);
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd{};
		csd.pSysMem = &constant;

		// Create constant buffer
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer));
	}

	void Update(Graphics& gfx, const C& constant)
	{
		INFOMAN(gfx);

		D3D11_MAPPED_SUBRESOURCE msr;
		GFX_THROW_INFO(GetContext(gfx)->Map(
			pConstantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr
		));
		memcpy(msr.pData, &constant, sizeof(constant));
		GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
	}

protected:
	ComPtr<ID3D11Buffer> pConstantBuffer;
	UINT slot;
};

template <typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;

	void Bind(Graphics& gfx) noexcpt override
	{
		this->GetContext(gfx)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
};

template <typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;

	void Bind(Graphics& gfx) noexcpt override
	{
		this->GetContext(gfx)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
};