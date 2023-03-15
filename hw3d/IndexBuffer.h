#pragma once

namespace Bind
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
		void Bind(Graphics& gfx) noexcpt override;
		UINT GetCount() const noexcept { return count; }

	protected:
		UINT count;
		ComPtr<ID3D11Buffer> pIndexBuffer;
	};
}
