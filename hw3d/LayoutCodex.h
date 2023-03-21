#pragma once
#include "Forwards.h"

namespace Dcb
{
	class LayoutCodex
	{
	public:
		static CookedLayout Resolve(RawLayout&& layout) noexcpt;

	private:
		static LayoutCodex& Get_() noexcept;
		std::unordered_map<std::string, std::shared_ptr<LayoutElement>> map;
	};
}
