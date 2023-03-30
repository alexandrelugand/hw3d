#include "stdafx.h"
#include "RenderQueuePass.h"

namespace Rgph
{
	void RenderQueuePass::Accept(Job job) noexcept
	{
		jobs.push_back(job);
	}

	void RenderQueuePass::Execute(Graphics& gfx) const noexcpt
	{
		BindAll(gfx);

		for (const auto& j : jobs)
		{
			j.Execute(gfx);
		}
	}

	void RenderQueuePass::Reset() noexcpt
	{
		jobs.clear();
	}
}
