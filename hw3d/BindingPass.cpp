#include "stdafx.h"
#include "BindingPass.h"

namespace Rgph
{
	BindingPass::BindingPass(std::string name, std::vector<std::shared_ptr<Bind::Bindable>> binds)
		: Pass(std::move(name)),
		  binds(std::move(binds))
	{
	}

	void BindingPass::AddBind(std::shared_ptr<Bind::Bindable> bind) noexcept
	{
		binds.push_back(std::move(bind));
	}

	void BindingPass::BindAll(Graphics& gfx) const noexcpt
	{
		BindBufferResources(gfx);
		for (auto& bind : binds)
		{
			bind->Bind(gfx);
		}
	}

	void BindingPass::Finalize()
	{
		Pass::Finalize();
		if (!renderTarget && !depthStencil)
			throw RGC_EXCEPTION("BindingPass [" + GetName() + "] needs at least one of renderTarget or depthStencil");
	}

	void BindingPass::BindBufferResources(Graphics& gfx) const noexcpt
	{
		if (renderTarget)
		{
			renderTarget->BindAsBuffer(gfx, depthStencil.get());
		}
		else
		{
			depthStencil->BindAsBuffer(gfx);
		}
	}
}
