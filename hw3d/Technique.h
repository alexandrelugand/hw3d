#pragma once
#include "Forwards.h"

class Technique
{
public:
	Technique() = default;
	Technique(std::string name) noexcept;

	void Submit(FrameCommander& frameCmder, const Draw::Drawable& drawable) const noexcept;

	const std::string& GetName() const noexcept;
	void AddStep(Step step) noexcept;
	bool IsActive() const noexcept;
	void SetActive(bool active_in) noexcept;

	void InitializeParentReferences(const Draw::Drawable& drawable) noexcept;
	void Accept(TechniqueProbe& probe);

private:
	bool active = true;
	std::vector<Step> steps{};
	std::string name = "Nameless Tech";
};
