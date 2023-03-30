#pragma once
#include "BufferResource.h"

namespace Bind
{
	class DepthStencil : public Bindable, public BufferResource
	{
		friend class RenderTarget;

	public:
		void BindAsBuffer(Graphics& gfx) noexcpt override;
		void BindAsBuffer(Graphics& gfx, BufferResource* buffer) noexcpt override;
		void BindAsBuffer(Graphics& gfx, RenderTarget* rt) noexcpt;
		void Clear(Graphics& gfx) noexcpt override;

	protected:
		DepthStencil(Graphics& gfx, UINT width, UINT height, bool canBindShaderInput);
		ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	};

	class ShaderInputDepthStencil : public DepthStencil
	{
	public:
		ShaderInputDepthStencil(Graphics& gfx, UINT slot);
		ShaderInputDepthStencil(Graphics& gfx, UINT width, UINT height, UINT slot);
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
