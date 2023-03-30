#pragma once

namespace Rgph
{
	class Job
	{
	public:
		Job(const Step* step, const Draw::Drawable* drawable);
		void Execute(Graphics& gfx) const noexcpt;

	private:
		const Step* pStep;
		const Draw::Drawable* pDrawable;
	};
}
