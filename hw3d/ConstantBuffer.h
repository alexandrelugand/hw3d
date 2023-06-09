#pragma once
#include "Bindable.h"
#include "Codex.h"

namespace Bind
{
	template <typename C>
	class ConstantBuffer : public Bindable
	{
	public:
		ConstantBuffer(Graphics& gfx, unsigned short slot): slot(slot)
		{
			INFOMAN(gfx);

			D3D11_BUFFER_DESC cbd;
			cbd.ByteWidth = sizeof(C);
			cbd.Usage = D3D11_USAGE_DYNAMIC;
			cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbd.MiscFlags = 0u;
			cbd.StructureByteStride = 0u;

			// Create constant buffer
			GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
		}

		ConstantBuffer(Graphics& gfx, const C& constant, unsigned int slot): slot(slot)
		{
			INFOMAN(gfx);

			D3D11_BUFFER_DESC cbd;
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

		~ConstantBuffer() override = default;

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
		unsigned int slot;
	};

	template <typename C>
	class VertexConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::pConstantBuffer;
		using ConstantBuffer<C>::slot;
		using ConstantBuffer<C>::GetInfoManager;
		using Bindable::GetContext;

	public:
		using ConstantBuffer<C>::ConstantBuffer;

		void Bind(Graphics& gfx) noexcpt override
		{
			INFOMAN_NOHR(gfx);
			GFX_THROW_INFO_ONLY(this->GetContext(gfx)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf()));
		}

		static std::shared_ptr<VertexConstantBuffer> Resolve(Graphics& gfx, const C& consts, unsigned int slot)
		{
			return Codex::Resolve<VertexConstantBuffer>(gfx, consts, slot);
		}

		static std::shared_ptr<VertexConstantBuffer> Resolve(Graphics& gfx, unsigned int slot)
		{
			return Codex::Resolve<VertexConstantBuffer>(gfx, slot);
		}

		static std::string GenerateUID(const C&, unsigned int slot)
		{
			return GenerateUID(slot);
		}

		static std::string GenerateUID(unsigned int slot)
		{
			return typeid(VertexConstantBuffer).name() + "#"s + std::to_string(slot);
		}

		std::string GetUID() const noexcept override
		{
			return GenerateUID(slot);
		}
	};

	template <typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::pConstantBuffer;
		using ConstantBuffer<C>::slot;
		using ConstantBuffer<C>::GetInfoManager;
		using Bindable::GetContext;

	public:
		using ConstantBuffer<C>::ConstantBuffer;

		void Bind(Graphics& gfx) noexcpt override
		{
			INFOMAN_NOHR(gfx);
			GFX_THROW_INFO_ONLY(this->GetContext(gfx)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf()));
		}

		static std::shared_ptr<PixelConstantBuffer> Resolve(Graphics& gfx, const C& consts, unsigned int slot)
		{
			return Codex::Resolve<PixelConstantBuffer>(gfx, consts, slot);
		}

		static std::shared_ptr<PixelConstantBuffer> Resolve(Graphics& gfx, unsigned int slot)
		{
			return Codex::Resolve<PixelConstantBuffer>(gfx, slot);
		}

		static std::string GenerateUID(const C&, unsigned int slot)
		{
			return GenerateUID(slot);
		}

		static std::string GenerateUID(unsigned int slot)
		{
			return typeid(PixelConstantBuffer).name() + "#"s + std::to_string(slot);
		}

		std::string GetUID() const noexcept override
		{
			return GenerateUID(slot);
		}
	};
}
