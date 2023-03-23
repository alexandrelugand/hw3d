#include "stdafx.h"
#include "Pass.h"

void Pass::Accept(Job& job) noexcept
{
	jobs.push_back(std::move(job));
}

void Pass::Execute(Graphics& gfx) const noexcept(!true)
{
	for (const auto& j : jobs)
	{
		j.Execute(gfx);
	}
}

void Pass::Reset() noexcept
{
	jobs.clear();
}
