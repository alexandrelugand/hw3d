#include "stdafx.h"
#include "Rasterizer.h"

namespace Bind
{
	Rasterizer::Rasterizer(Graphics& gfx, CullMode cull)
		: cull(cull)
	{
	}

	void Rasterizer::Bind(Graphics& gfx) noexcept
	{
		gfx.SetCullMode(cull);
	}

	std::shared_ptr<Rasterizer> Rasterizer::Resolve(Graphics& gfx, CullMode cull)
	{
		return Codex::Resolve<Rasterizer>(gfx, cull);
	}

	std::string Rasterizer::GenerateUID(CullMode cull)
	{
		using namespace std::string_literals;
		return typeid(Rasterizer).name() + "#"s + (cull == CullMode::None ? "2s" : "1s");
	}

	std::string Rasterizer::GetUID() const noexcept
	{
		return GenerateUID(cull);
	}
}
