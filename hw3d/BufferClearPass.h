#pragma once

namespace Rgph
{
	class BufferClearPass : public Pass
	{
	public:
		BufferClearPass(std::string name);
		void Execute(Graphics& gfx) const noexcpt override;

	private:
		std::shared_ptr<Bind::BufferResource> buffer;
	};
}
