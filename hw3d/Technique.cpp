#include "stdafx.h"
#include "Technique.h"

Technique::Technique(std::string name) noexcept
	: name(std::move(name))
{
}

void Technique::Submit(FrameCommander& frameCmder, const Draw::Drawable& drawable) const noexcept
{
	if (active)
	{
		for (const auto& s : steps)
		{
			s.Submit(frameCmder, drawable);
		}
	}
}

const std::string& Technique::GetName() const noexcept
{
	return name;
}

void Technique::AddStep(Step step) noexcept
{
	steps.push_back(std::move(step));
}

bool Technique::IsActive() const noexcept
{
	return active;
}

void Technique::SetActive(bool active_in) noexcept
{
	active = active_in;
}

void Technique::InitializeParentReferences(const Draw::Drawable& drawable) noexcept
{
	for (auto& s : steps)
	{
		s.InitializeParentReferences(drawable);
	}
}

void Technique::Accept(TechniqueProbe& probe)
{
	probe.SetTechnique(this);
	for (auto& s : steps)
	{
		s.Accept(probe);
	}
}
