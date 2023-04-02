#pragma once
#include "BufferResource.h"

namespace Bind
{
	class DepthStencil : public Bindable, public BufferResource
	{
		friend class RenderTarget;

	public:
		enum class Usage
		{
			DepthStencil,
			ShadowDepth
		};

		void BindAsBuffer(Graphics& gfx) noexcpt override;
		void BindAsBuffer(Graphics& gfx, BufferResource* buffer) noexcpt override;
		void BindAsBuffer(Graphics& gfx, RenderTarget* rt) noexcpt;
		void Clear(Graphics& gfx) noexcpt override;
		Surface ToSurface(Graphics& gfx, bool linearlize = true) const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

	protected:
		DepthStencil(Graphics& gfx, UINT width, UINT height, bool canBindShaderInput, Usage usage);
		ComPtr<ID3D11DepthStencilView> pDepthStencilView;
		unsigned int width;
		unsigned int height;
	};

	class ShaderInputDepthStencil : public DepthStencil
	{
	public:
		ShaderInputDepthStencil(Graphics& gfx, UINT slot, Usage usage = Usage::DepthStencil);
		ShaderInputDepthStencil(Graphics& gfx, UINT width, UINT height, UINT slot, Usage usage = Usage::DepthStencil);
		void Bind(Graphics& gfx) noexcpt override;

	private:
		UINT slot;
		ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
	};

	class OutputOnlyDepthStencil : public DepthStencil
	{
	public:
		OutputOnlyDepthStencil(Graphics& gfx);
		OutputOnlyDepthStencil(Graphics& gfx, UINT width, UINT height);
		void Bind(Graphics& gfx) noexcpt override;
	};
}
