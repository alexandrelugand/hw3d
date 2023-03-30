#pragma once
#include "Forwards.h"

namespace Bind
{
	class Bindable : public GraphicsResource
	{
	public:
		virtual void Bind(Graphics& gfx) noexcpt = 0;
		virtual ~Bindable() = default;

		virtual std::string GetUID() const noexcept
		{
			assert(false);
			return "";
		}

		virtual void InitializeParentReferences(const Draw::Drawable& drawable) noexcept
		{
		}

		virtual void Accept(Probes::TechniqueProbe& probe)
		{
		}
	};

	class CloningBindable : public Bindable
	{
	public:
		virtual std::unique_ptr<CloningBindable> Clone() const noexcept = 0;
	};
}
