#include "stdafx.h"
#include "Cull.h"

namespace Bind
{
	Cull::Cull(Graphics& gfx, CullMode cullMode)
		: cullMode(cullMode)
	{
	}

	void Cull::Bind(Graphics& gfx) noexcpt
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(gfx.SetCullMode(cullMode));
	}

	std::shared_ptr<Cull> Cull::Resolve(Graphics& gfx, CullMode cullMode)
	{
		return Codex::Resolve<Cull>(gfx, cullMode);
	}

	std::string Cull::GenerateUID(CullMode cullMode)
	{
		return typeid(Cull).name() + "#"s + std::to_string(cullMode);
	}

	std::string Cull::GetUID() const noexcept
	{
		return GenerateUID(cullMode);
	}
}
