#include "stdafx.h"
#include "Job.h"

namespace Rgph
{
	Job::Job(const Step* step, const Draw::Drawable* drawable)
		: pStep(step), pDrawable(drawable)
	{
	}

	void Job::Execute(Graphics& gfx) const noexcpt
	{
		pDrawable->Bind(gfx);
		pStep->Bind(gfx);
		gfx.DrawIndexed(pDrawable->GetIndexCount());
	}
}
