#include "stdafx.h"
#include "SolidSphere.h"


#include "Sphere.h"

namespace Draw
{
	SolidSphere::SolidSphere(Graphics& gfx, float radius, XMFLOAT3 color)
		: ColorDrawable(color)
	{
		auto model = Geometry::Sphere::Make(12, 24);
		model.Transform(XMMatrixScaling(radius, radius, radius));

		const auto tag = "$sphere." + Uuid::ToString(Uuid::New());
		pVertices = Bind::VertexBuffer::Resolve(gfx, tag, model.vertices);
		pIndices = Bind::IndexBuffer::Resolve(gfx, tag, model.indices);
		pTopology = Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		{
			Rgph::Technique solid{Chan::main};
			Rgph::Step only("lambertian");

			auto pvs = Bind::VertexShader::Resolve(gfx, "Solid_VS.cso");
			only.AddBindable(Bind::InputLayout::Resolve(gfx, model.vertices.GetLayout(), *pvs));
			only.AddBindable(std::move(pvs));

			only.AddBindable(Bind::PixelShader::Resolve(gfx, "Solid_PS.cso"));

			only.AddBindable(std::make_shared<Bind::TransformCBuf>(gfx));
			only.AddBindable(std::make_shared<Bind::ColorCBuf>(gfx, *this, 1u));

			only.AddBindable(Bind::Blender::Resolve(gfx, false));
			only.AddBindable(Bind::Rasterizer::Resolve(gfx));

			solid.AddStep(std::move(only));
			AddTechnique(std::move(solid));
		}
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
