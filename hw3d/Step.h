#pragma once
#include "Forwards.h"
#include "RenderQueuePass.h"

namespace Rgph
{
	class Step
	{
	public:
		Step(std::string targetPassName);
		Step(Step&&) = default;
		Step(const Step& src) noexcept;
		Step& operator=(const Step&) = delete;
		Step& operator=(Step&&) = delete;

		void AddBindable(std::shared_ptr<Bind::Bindable> bind_in) noexcpt;
		void Submit(const Draw::Drawable& drawable) const;
		void Bind(Graphics& gfx) const noexcpt;

		void InitializeParentReferences(const Draw::Drawable& parent) const noexcept;
		void Accept(Probes::TechniqueProbe& probe);
		void Link(RenderGraph& rg);

	private:
		std::string targetPassName;
		std::vector<std::shared_ptr<Bind::Bindable>> bindables{};
		RenderQueuePass* pTargetPass = nullptr;
	};
}
