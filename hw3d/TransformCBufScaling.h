#pragma once
#include "DynamicConstant.h"

namespace Bind
{
	class TransformCBufScaling : public TransformCBuf
	{
	public:
		TransformCBufScaling(Graphics& gfx, float scale);

		void Accept(Probes::TechniqueProbe& probe) override;
		void Bind(Graphics& gfx) noexcpt override;
		std::unique_ptr<CloningBindable> Clone() const noexcept override;

	private:
		static Dcb::RawLayout MakeLayout();
		Dcb::Buffer buf;
	};
}
