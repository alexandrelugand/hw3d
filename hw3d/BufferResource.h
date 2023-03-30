#pragma once

namespace Bind
{
	class BufferResource
	{
	public:
		virtual ~BufferResource() = default;
		virtual void BindAsBuffer(Graphics& gfx) noexcpt = 0;
		virtual void BindAsBuffer(Graphics& gfx, BufferResource* buffer) noexcpt = 0;
		virtual void Clear(Graphics& gfx) noexcpt = 0;
	};
}
