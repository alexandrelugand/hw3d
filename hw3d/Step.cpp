#include "stdafx.h"
#include "Step.h"

Step::Step(size_t targetPass_in)
	: targetPass(targetPass_in)
{
}

Step::Step(const Step& src) noexcept
	: targetPass(src.targetPass)
{
	bindables.reserve(src.bindables.size());
	for (auto& pb : src.bindables)
	{
		if (auto* pCloning = dynamic_cast<const Bind::CloningBindable*>(pb.get()))
		{
			bindables.push_back(pCloning->Clone());
		}
		else
		{
			bindables.push_back(pb);
		}
	}
}

void Step::AddBindable(std::shared_ptr<Bind::Bindable> bind_in) noexcpt
{
	bindables.push_back(std::move(bind_in));
}

void Step::Submit(FrameCommander& frameCmder, const Draw::Drawable& drawable) const
{
	auto j = Job{this, &drawable};
	frameCmder.Accept(j, targetPass);
}

void Step::Bind(Graphics& gfx) const
{
	for (const auto& b : bindables)
	{
		b->Bind(gfx);
	}
}

void Step::InitializeParentReferences(const Draw::Drawable& parent) noexcept
{
	for (auto& b : bindables)
	{
		b->InitializeParentReferences(parent);
	}
}

void Step::Accept(TechniqueProbe& probe)
{
	probe.SetStep(this);
	for (const auto& b : bindables)
	{
		b->Accept(probe);
	}
}
