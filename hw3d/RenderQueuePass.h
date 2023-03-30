#pragma once

namespace Rgph
{
	class RenderQueuePass : public BindingPass
	{
	public:
		using BindingPass::BindingPass;
		void Accept(Job job) noexcept;
		void Execute(Graphics& gfx) const noexcpt override;
		void Reset() noexcpt override;

	private:
		std::vector<Job> jobs{};
	};
}
