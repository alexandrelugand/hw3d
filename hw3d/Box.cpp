#include "stdafx.h"
#include "Box.h"
#include "Cube.h"

namespace Draw
{
	Box::Box(Graphics& gfx, XMFLOAT3 material, const float scale, const XMFLOAT3& position)
		: DrawableObject(gfx, scale, position)
	{
		name = "Box";
		const auto tag = std::format("${}.{}", String::ToLower(name).c_str(), std::to_string(id));
		auto model = Geometry::Cube::Make();
		model.SetNormalsIndependentFlat();

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
