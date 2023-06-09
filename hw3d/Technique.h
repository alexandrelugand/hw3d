#pragma once
#include "Forwards.h"

namespace Rgph
{
	class Technique
	{
	public:
		Technique(size_t channels);
		Technique(std::string name, size_t channels, bool startActive = true) noexcept;

		void Submit(const Draw::Drawable& drawable, size_t channelFilter) const noexcept;

		const std::string& GetName() const noexcept;
		void AddStep(Step step) noexcept;
		bool IsActive() const noexcept;
		void SetActive(bool active_in) noexcept;

		void InitializeParentReferences(const Draw::Drawable& drawable) const noexcept;
		void Accept(Probes::TechniqueProbe& probe);
		void Link(RenderGraph& rg);

	private:
		bool active = true;
		std::vector<Step> steps{};
		std::string name;
		size_t channels;
	};
}
