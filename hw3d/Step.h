#pragma once
#include "Forwards.h"

class Step
{
public:
	Step(size_t targetPass_in);
	Step(Step&&) = default;
	Step(const Step& src) noexcept;

	Step& operator=(const Step&) = delete;
	Step& operator=(Step&&) = delete;

	void AddBindable(std::shared_ptr<Bind::Bindable> bind_in) noexcpt;
	void Submit(FrameCommander& frameCmder, const Draw::Drawable& drawable) const;
	void Bind(Graphics& gfx) const;

	void InitializeParentReferences(const Draw::Drawable& parent) noexcept;
	void Accept(TechniqueProbe& probe);

private:
	size_t targetPass;
	std::vector<std::shared_ptr<Bind::Bindable>> bindables{};
};
