#pragma once
#include "Forwards.h"

namespace Bind
{
	class Texture : public Bindable
	{
	public:
		Texture(Graphics& gfx, const std::string& path, unsigned int slot = 0);
		void Bind(Graphics& gfx) noexcpt override;

		static std::shared_ptr<Texture> Resolve(Graphics& gfx, const std::string& path, UINT slot = 0);
		static std::string GenerateUID(const std::string& path, UINT slot = 0);
		std::string GetUID() const noexcept override;
		bool HasAlpha() const noexcept;

	protected:
		bool hasAlpha = false;
		std::string path;
		ComPtr<ID3D11ShaderResourceView> pTextureView;

	private:
		unsigned int slot;
		static UINT CalculateNumberOfMipLevels(UINT width, UINT height) noexcept;
	};
}
