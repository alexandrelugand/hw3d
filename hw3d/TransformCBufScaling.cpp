#include "stdafx.h"
#include "TransformCBufScaling.h"

namespace Bind
{
	TransformCBufScaling::TransformCBufScaling(Graphics& gfx, float scale)
		: TransformCBuf(gfx, Shaders::CBuf::Scaling), buf(MakeLayout())
	{
		buf["scale"] = scale;
	}

	void TransformCBufScaling::Accept(Probes::TechniqueProbe& probe)
	{
		probe.VisitBuffer(buf);
	}

	void TransformCBufScaling::Bind(Graphics& gfx) noexcpt
	{
		const float scale = buf["scale"];

		const auto scaleMatrix = XMMatrixScaling(scale, scale, scale);
		auto xf = GetTransforms(gfx);
		xf.modelView = xf.modelView * scaleMatrix;
		xf.modelViewProj = xf.modelViewProj * scaleMatrix;

		UpdateBindImpl(gfx, xf);
	}

	std::unique_ptr<CloningBindable> TransformCBufScaling::Clone() const noexcept
	{
		return std::make_unique<TransformCBufScaling>(*this);
	}

	Dcb::RawLayout TransformCBufScaling::MakeLayout()
	{
		Dcb::RawLayout layout;
		layout.Add<Dcb::Float>("scale");
		return layout;
	}
}
