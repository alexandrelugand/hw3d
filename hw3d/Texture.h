#pragma once
#include "Forwards.h"

namespace Bind
{
	class Texture : public Bindable
	{
	public:
		Texture(Graphics& gfx, const std::string& path, unsigned int slot);
		void Bind(Graphics& gfx) noexcpt override;

		static std::shared_ptr<Texture> Resolve(Graphics& gfx, const std::string& path, unsigned int slot);
		static std::string GenerateUID(const std::string& path, unsigned int slot);
		std::string GetUID() const noexcept override;
		bool HasAlpha() const noexcept;

	protected:
		bool hasAlpha = false;
		std::string path;
		ComPtr<ID3D11ShaderResourceView> pTextureView;

	private:
		unsigned int slot;
		static unsigned int CalculateNumberOfMipLevels(unsigned int width, unsigned int height) noexcept;
	};
}
