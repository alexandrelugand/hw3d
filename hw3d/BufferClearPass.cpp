#include "stdafx.h"
#include "BufferClearPass.h"

namespace Rgph
{
	BufferClearPass::BufferClearPass(std::string name)
		: Pass(std::move(name))
	{
		RegisterSink(DirectBufferSink<Bind::BufferResource>::Make("buffer", buffer));
		RegisterSource(DirectBufferSource<Bind::BufferResource>::Make("buffer", buffer));
	}

	void BufferClearPass::Execute(Graphics& gfx) const noexcpt
	{
		buffer->Clear(gfx);
	}
}
