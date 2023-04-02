#include "stdafx.h"
#include "Technique.h"

namespace Rgph
{
	Technique::Technique(size_t channels)
		: channels(channels)
	{
	}

	Technique::Technique(std::string name, size_t channels, bool startActive) noexcept
		: active(startActive), name(name), channels(channels)
	{
	}

	void Technique::Submit(const Draw::Drawable& drawable, size_t channelFilter) const noexcept
	{
		if (active && ((channels & channelFilter) != 0))
		{
			for (const auto& s : steps)
			{
				s.Submit(drawable);
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

	void Technique::InitializeParentReferences(const Draw::Drawable& drawable) const noexcept
	{
		for (auto& s : steps)
		{
			s.InitializeParentReferences(drawable);
		}
	}

	void Technique::Accept(Probes::TechniqueProbe& probe)
	{
		probe.SetTechnique(this);
		for (auto& s : steps)
		{
			s.Accept(probe);
		}
	}

	void Technique::Link(RenderGraph& rg)
	{
		for (auto& step : steps)
		{
			step.Link(rg);
		}
	}
}
