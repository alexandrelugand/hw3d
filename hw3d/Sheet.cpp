#include "stdafx.h"
#include "Sheet.h"
#include "Plane.h"
#include "TransformAllCBuf.h"

namespace Draw
{
	Sheet::Sheet(Graphics& gfx, float scale, const XMFLOAT3& position)
		: DrawableObject(gfx, scale, position)
	{
		name = "Sheet";
		const auto tag = std::format("${}.{}", String::ToLower(name).c_str(), std::to_string(id));
		auto model = Geometry::Plane::Make(1, 1);
		model.SetNormalsIndependentFlat();

		pVertices = Bind::VertexBuffer::Resolve(gfx, tag, model.vertices);
		pIndices = Bind::IndexBuffer::Resolve(gfx, tag, model.indices);
		pTopology = Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		{
			Technique shade("Shade");
			{
				Rgph::Step only("lambertian");

				only.AddBindable(Bind::Texture::Resolve(gfx, "images\\brickwall.jpg"));
				only.AddBindable(Bind::Texture::Resolve(gfx, "images\\brickwall_normal_obj.png", 2u));
				only.AddBindable(Bind::Sampler::Resolve(gfx));

				auto pvs = Bind::VertexShader::Resolve(gfx, "PhongDif_VS.cso");
				only.AddBindable(std::make_unique<Bind::InputLayout>(gfx, model.vertices.GetLayout(), *pvs));
				only.AddBindable(std::move(pvs));

				only.AddBindable(Bind::PixelShader::Resolve(gfx, "PhongDifNrmNoTBN_PS.cso"));

				Dcb::RawLayout lay;
				lay.Add<Dcb::Float3>("specularColor");
				lay.Add<Dcb::Float>("specularWeight");
				lay.Add<Dcb::Float>("specularGloss");
				lay.Add<Dcb::Bool>("useNormalMap");
				lay.Add<Dcb::Float>("normalMapWeight");
				auto buf = Dcb::Buffer(std::move(lay));
				buf["specularColor"] = XMFLOAT3{0.7f, 0.7f, 0.7f};
				buf["specularWeight"] = 0.1f;
				buf["specularGloss"] = 20.0f;
				buf["useNormalMap"] = true;
				buf["normalMapWeight"] = 1.0f;
				only.AddBindable(std::make_shared<Bind::CachingPixelCBuf>(gfx, buf, 1u));

				only.AddBindable(std::make_shared<Bind::TransformAllCBuf>(gfx, 0u, 2u));
				only.AddBindable(std::make_shared<Bind::Rasterizer>(gfx, CullMode::None));

				shade.AddStep(std::move(only));
			}
			AddTechnique(std::move(shade));
		}
	}
}
