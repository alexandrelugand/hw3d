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

		//static std::shared_ptr<Stencil> Resolve( Graphics& gfx,bool blending,std::optional<float> factor = {} );
		//static std::string GenerateUID( bool blending,std::optional<float> factor );
		//std::string GetUID() const noexcept override;

	private:
		ComPtr<ID3D11DepthStencilState> pStencil;
	};
}
