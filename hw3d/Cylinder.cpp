#include "stdafx.h"
#include "Cylinder.h"
#include "Prism.h"

namespace Draw
{
	Cylinder::Cylinder(Graphics& gfx, XMFLOAT3 material, float scale, const XMFLOAT3& position)
		: DrawableObject(gfx, scale, position)
	{
		name = "Cylinder";
		const auto tag = std::format("${}.{}", String::ToLower(name).c_str(), std::to_string(id));
		const auto model = Geometry::Prism::MakeTesselatedIndependentNormals(24);

		pVertices = Bind::VertexBuffer::Resolve(gfx, tag, model.vertices);
		pIndices = Bind::IndexBuffer::Resolve(gfx, tag, model.indices);
		pTopology = Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		{
			Technique shade("Shade");
			{
				Rgph::Step only("lambertian");

				auto pvs = Bind::VertexShader::Resolve(gfx, "BlendedPhong_VS.cso");
				only.AddBindable(std::make_unique<Bind::InputLayout>(gfx, model.vertices.GetLayout(), *pvs));
				only.AddBindable(std::move(pvs));

				only.AddBindable(Bind::PixelShader::Resolve(gfx, "BlendedPhong_PS.cso"));

				Dcb::RawLayout lay;
				lay.Add<Dcb::Float3>("color");
				lay.Add<Dcb::Float>("specularIntensity");
				lay.Add<Dcb::Float>("specularPower");
				auto buf = Dcb::Buffer(std::move(lay));
				buf["color"] = material;
				buf["specularIntensity"] = 0.1f;
				buf["specularPower"] = 20.0f;
				only.AddBindable(std::make_shared<Bind::CachingPixelCBuf>(gfx, buf, 1u));

				only.AddBindable(std::make_shared<Bind::TransformCBuf>(gfx));

				shade.AddStep(std::move(only));
			}
			AddTechnique(std::move(shade));
		}
	}
}
