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

		virtual void InitializeParentReferences(const Draw::Drawable& drawable) noexcept
		{
		}

		virtual void Accept(TechniqueProbe& probe)
		{
		}

	protected:
		static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;
		static ID3D11Device* GetDevice(Graphics& gfx) noexcept;
		static DxgiInfoManager& GetInfoManager(Graphics& gfx);
	};

	class CloningBindable : public Bindable
	{
	public:
		virtual std::unique_ptr<CloningBindable> Clone() const noexcept = 0;
	};
}
