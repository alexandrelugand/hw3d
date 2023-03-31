#include "stdafx.h"
#include "Step.h"

namespace Rgph
{
	Step::Step(std::string targetPassName)
		: targetPassName(std::move(targetPassName))
	{
	}

	Step::Step(const Step& src) noexcept
		: targetPassName(src.targetPassName)
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

	void Step::Submit(const Draw::Drawable& drawable) const
	{
		pTargetPass->Accept(Job{this, &drawable});
	}

	void Step::Bind(Graphics& gfx) const noexcpt
	{
		for (const auto& b : bindables)
		{
			b->Bind(gfx);
		}
	}

	void Step::InitializeParentReferences(const Draw::Drawable& parent) const noexcept
	{
		for (auto& b : bindables)
		{
			b->InitializeParentReferences(parent);
		}
	}

	void Step::Accept(Probes::TechniqueProbe& probe)
	{
		probe.SetStep(this);
		for (const auto& b : bindables)
		{
			b->Accept(probe);
		}
	}

	void Step::Link(RenderGraph& rg)
	{
		assert(pTargetPass == nullptr);
		pTargetPass = &rg.GetRenderQueue(targetPassName);
	}
}
