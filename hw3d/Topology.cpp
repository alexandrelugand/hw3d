#include "stdafx.h"
#include "Topology.h"

namespace Bind
{
	Topology::Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
		: type(type)
	{
	}

	void Topology::Bind(Graphics& gfx) noexcpt
	{
		INFOMAN_NOHR(gfx);
		GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetPrimitiveTopology(type));
	}
}
