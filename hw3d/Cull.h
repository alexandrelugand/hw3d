#pragma once

namespace Bind
{
	class Cull : public Bindable
	{
	public:
		Cull(Graphics& gfx, CullMode cullMode);
		void Bind(Graphics& gfx) noexcpt override;

		static std::shared_ptr<Cull> Resolve(Graphics& gfx, CullMode cullMode);
		static std::string GenerateUID(CullMode cullMode);
		std::string GetUID() const noexcept override;

	private:
		CullMode cullMode;
	};
}
