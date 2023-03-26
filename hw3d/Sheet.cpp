#include "stdafx.h"
#include "Sheet.h"
#include "Plane.h"
#include "TransformAllCBuf.h"

namespace Draw
{
	Sheet::Sheet(Graphics& gfx, float scale, const XMFLOAT3& position)
		: DrawableObject(gfx, scale, position)
	{
		const auto tag = "$sheet." + Uuid::ToString(Uuid::New());
		auto model = Geometry::Plane::Make(1, 1);
		model.SetNormalsIndependentFlat();

		pVertices = Bind::VertexBuffer::Resolve(gfx, tag, model.vertices);
		pIndices = Bind::IndexBuffer::Resolve(gfx, tag, model.indices);
		pTopology = Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		{
			Technique shade("Shade");
			{
				Step only(0);

				only.AddBindable(Bind::Texture::Resolve(gfx, "images\\brickwall.jpg"));
				only.AddBindable(Bind::Texture::Resolve(gfx, "images\\brickwall_normal_obj.png", 2u));
				only.AddBindable(Bind::Sampler::Resolve(gfx));

				auto pvs = Bind::VertexShader::Resolve(gfx, "PhongDif_VS.cso");
				auto pvsbc = pvs->GetBytecode();
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

				only.AddBindable(std::make_unique<Bind::InputLayout>(gfx, model.vertices.GetLayout(), pvsbc));
				only.AddBindable(std::make_shared<Bind::TransformAllCBuf>(gfx, 0u, 2u));
				only.AddBindable(std::make_shared<Bind::Rasterizer>(gfx, None));

				shade.AddStep(std::move(only));
			}
			AddTechnique(std::move(shade));
		}
	}

	bool Sheet::SpawnControlWindow() noexcept
	{
		bool open = true;
		if (ImGui::Begin("Sheet", &open))
		{
			ImGui::Text("Position");
			ImGui::SliderFloat("X", &pos.x, -80.0f, 80.0f, "%.1f");
			ImGui::SliderFloat("Y", &pos.y, -80.0f, 80.0f, "%.1f");
			ImGui::SliderFloat("Z", &pos.z, -80.0f, 80.0f, "%.1f");

			ImGui::Text("Rotation");
			ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
			ImGui::SliderAngle("Phi", &phi, -180.0f, 180.0f);

			ImGui::Text("Orientation");
			ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
			ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
			ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);

			class Probe : public TechniqueProbe
			{
			public:
				void OnSetTechnique() override
				{
					using namespace std::string_literals;
					ImGui::TextColored({0.4f, 1.0f, 0.6f, 1.0f}, pTech->GetName().c_str());
					bool active = pTech->IsActive();
					ImGui::Checkbox(("Tech Active##"s + std::to_string(techIdx)).c_str(), &active);
					pTech->SetActive(active);
				}

				bool OnVisitBuffer(Dcb::Buffer& buf) override
				{
					float dirty = false;
					const auto dcheck = [&dirty](bool changed) { dirty = dirty || changed; };
					auto tag = [tagScratch = std::string{}, tagString = "##" + std::to_string(bufIdx)]
					(const char* label) mutable
					{
						tagScratch = label + tagString;
						return tagScratch.c_str();
					};

					if (auto v = buf["useNormalMap"]; v.Exists())
					{
						dcheck(ImGui::Checkbox(tag("Normal map"), &v));
					}
					if (auto v = buf["specularColor"]; v.Exists())
					{
						dcheck(ImGui::ColorPicker3(tag("Spec. Color"), reinterpret_cast<float*>(&static_cast<XMFLOAT3&>(v))));
					}
					if (auto v = buf["specularWeight"]; v.Exists())
					{
						dcheck(ImGui::SliderFloat(tag("Spec. Weight"), &v, 0.0f, 1.0f));
					}
					if (auto v = buf["specularGloss"]; v.Exists())
					{
						dcheck(ImGui::SliderFloat(tag("Glossiness"), &v, 1.0f, 100.0f, "%.1f"));
					}
					return dirty;
				}
			} probe;

			Accept(probe);

			if (ImGui::Button("Reset"))
				Reset();
		}
		ImGui::End();

		return open;
	}
}
