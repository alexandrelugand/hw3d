#pragma once

namespace Bind
{
	class Topology : public Bindable
	{
	public:
		Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
		void Bind(Graphics& gfx) noexcpt override;

		static std::shared_ptr<Topology> Resolve(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type);
		std::string GetUID() const noexcept override;

	private:
		D3D11_PRIMITIVE_TOPOLOGY type;
	};
}
