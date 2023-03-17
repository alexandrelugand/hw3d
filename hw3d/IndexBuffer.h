#pragma once

namespace Bind
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
		IndexBuffer(Graphics& gfx, std::string tag, const std::vector<unsigned short>& indices);
		void Bind(Graphics& gfx) noexcpt override;
		UINT GetCount() const noexcept { return count; }

		static std::shared_ptr<IndexBuffer> Resolve(Graphics& gfx, const std::string& tag, const std::vector<unsigned short>& indices);

		template <typename... Ignore>
		static std::string GenerateUID(const std::string& tag, Ignore&&... ignore)
		{
			return GenerateUID_(tag);
		}

		std::string GetUID() const noexcept override;

	protected:
		std::string tag;
		UINT count;
		ComPtr<ID3D11Buffer> pIndexBuffer;

	private:
		static std::string GenerateUID_(const std::string& tag);
	};
}
