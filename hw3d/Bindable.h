#pragma once
#include "Forwards.h"

namespace Bind
{
	class Bindable
	{
	public:
		virtual void Bind(Graphics& gfx) noexcpt = 0;
		virtual ~Bindable() = default;

		virtual std::string GetUID() const noexcept
		{
			assert(false);
			return "";
		}

	protected:
		static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;
		static ID3D11Device* GetDevice(Graphics& gfx) noexcept;
		static DxgiInfoManager& GetInfoManager(Graphics& gfx);
	};
}
