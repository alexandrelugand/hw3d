#pragma once
#include "VertexLayout.h"

namespace Bind
{
	class VertexBuffer : public Bindable
	{
	public:
		VertexBuffer(Graphics& gfx, const std::string& tag, const Dvtx::VertexBufferDescriptor& vbd);
		VertexBuffer(Graphics& gfx, const Dvtx::VertexBufferDescriptor& vbd);
		void Bind(Graphics& gfx) noexcpt override;
		const Dvtx::VertexLayout& GetLayout() const noexcept;

		static std::shared_ptr<VertexBuffer> Resolve(Graphics& gfx, const std::string& tag, const Dvtx::VertexBufferDescriptor& vbd);

		template <typename... Ignore>
		static std::string GenerateUID(const std::string& tag, Ignore&&... ignore)
		{
			return GenerateUID_(tag);
		}

		std::string GetUID() const noexcept override;

	protected:
		std::string tag;
		UINT stride;
		ComPtr<ID3D11Buffer> pVertexBuffer;
		Dvtx::VertexLayout layout;

	private:
		static std::string GenerateUID_(const std::string& tag);
	};
}
