#pragma once

namespace Bind
{
	class Stencil : public Bindable
	{
	public:
		enum class Mode
		{
			Off,
			Write,
			Mask
		};

		Stencil(Graphics& gfx, Mode mode);

		void Bind(Graphics& gfx) noexcept override;

		static std::shared_ptr<Stencil> Resolve(Graphics& gfx, Mode mode);
		static std::string GenerateUID(Mode mode);
		std::string GetUID() const noexcept override;

	private:
		Mode mode;
		ComPtr<ID3D11DepthStencilState> pStencil;
	};
}
