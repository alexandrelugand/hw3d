#include "stdafx.h"
#include "SkinnedBox.h"
#include "Cube.h"

namespace Draw
{
	SkinnedBox::SkinnedBox(Graphics& gfx, float scale, XMFLOAT3 position)
		: DrawableObject(gfx, scale, position)
	{
		name = "SkinnedBox";
		const auto tag = std::format("${}.{}", String::ToLower(name).c_str(), std::to_string(id));
		auto model = Geometry::Cube::MakeIndependentTexturedAndTBN();
		model.SetNormalsAndTBNIndependentFlat();
		model.Transform(XMMatrixScaling(scale, scale, scale));

		pVertices = Bind::VertexBuffer::Resolve(gfx, tag, model.vertices);
		pIndices = Bind::IndexBuffer::Resolve(gfx, tag, model.indices);
		pTopology = Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		auto tcb = std::make_shared<Bind::TransformCBuf>(gfx);

		{
			Rgph::Technique shade("Shade");
			{
				Rgph::Step only("lambertian");

				only.AddBindable(Bind::Texture::Resolve(gfx, "Images\\brickwall.jpg"));
				only.AddBindable(Bind::Texture::Resolve(gfx, "images\\brickwall_normal_obj.png", 2u));
				only.AddBindable(Bind::Sampler::Resolve(gfx));

				auto pvs = Bind::VertexShader::Resolve(gfx, "PhongDifNrm_VS.cso");
				only.AddBindable(Bind::InputLayout::Resolve(gfx, model.vertices.GetLayout(), *pvs));
				only.AddBindable(std::move(pvs));

				only.AddBindable(Bind::PixelShader::Resolve(gfx, "PhongDifNrm_PS.cso"));

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

				only.AddBindable(Bind::Rasterizer::Resolve(gfx, CullMode::Back));
				only.AddBindable(tcb);

				shade.AddStep(std::move(only));
			}
			AddTechnique(std::move(shade));
		}

		Rgph::Technique outline("Outline", false);
		{
			Rgph::Step mask("outlineMask");
			{
				// TODO: better sub-layout generation tech for future consideration maybe
				mask.AddBindable(Bind::InputLayout::Resolve(gfx, model.vertices.GetLayout(), *Bind::VertexShader::Resolve(gfx, "Solid_VS.cso")));

				mask.AddBindable(std::move(tcb));

				// TODO: might need to specify rasterizer when doubled-sided models start being used
				outline.AddStep(std::move(mask));
			}

			Rgph::Step draw("outlineDraw");
			{
				Dcb::RawLayout lay;
				lay.Add<Dcb::Float4>("color");
				auto buf = Dcb::Buffer(std::move(lay));
				buf["color"] = XMFLOAT4{1.0f, 0.4f, 0.4f, 1.0f};
				draw.AddBindable(std::make_shared<Bind::CachingPixelCBuf>(gfx, buf, 1u));

				// TODO: better sub-layout generation tech for future consideration maybe
				draw.AddBindable(Bind::InputLayout::Resolve(gfx, model.vertices.GetLayout(), *Bind::VertexShader::Resolve(gfx, "Solid_VS.cso")));

				draw.AddBindable(std::make_shared<Bind::TransformCBuf>(gfx));

				// TODO: might need to specify rasterizer when doubled-sided models start being used
				outline.AddStep(std::move(draw));
			}
			AddTechnique(std::move(outline));
		}
	}
}
