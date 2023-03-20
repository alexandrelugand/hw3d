#include "stdafx.h"
#include "SolidSphere.h"

#include "Blender.h"
#include "Rasterizer.h"
#include "Sphere.h"

namespace Draw
{
	SolidSphere::SolidSphere(Graphics& gfx, float radius, XMFLOAT3 color)
		: ColorDrawable(color)
	{
		auto model = Geometry::Sphere::Make(12, 24);
		model.Transform(XMMatrixScaling(radius, radius, radius));

		const auto tag = "$sphere." + Uuid::ToString(Uuid::New());
		AddBind(Bind::VertexBuffer::Resolve(gfx, tag, model.vbd));
		AddBind(Bind::IndexBuffer::Resolve(gfx, tag, model.indices));

		auto pvs = Bind::VertexShader::Resolve(gfx, "SolidVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddBind(std::move(pvs));

		AddBind(Bind::PixelShader::Resolve(gfx, "SolidPS.cso"));

		AddBind(Bind::InputLayout::Resolve(gfx, model.vbd.GetLayout(), pvsbc));

		AddBind(Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		AddBind(std::make_shared<Bind::TransformCBuf>(gfx, *this));
		AddBind(std::make_shared<Bind::ColorCBuf>(gfx, *this));

		AddBind(Bind::Blender::Resolve(gfx, false));
		AddBind(Bind::Rasterizer::Resolve(gfx));
	}

	void SolidSphere::SetPos(XMFLOAT3 pos) noexcept
	{
		position = pos;
	}

	XMMATRIX SolidSphere::GetTransform() const noexcept
	{
		return XMMatrixTranslation(position.x, position.y, position.z);
	}
}
