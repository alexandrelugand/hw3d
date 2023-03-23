#include "stdafx.h"
#include "SkinnedBox.h"
#include "Cube.h"

namespace Draw
{
	SkinnedBox::SkinnedBox(Graphics& gfx, float scale, XMFLOAT3 position)
		: DrawableObject(gfx)
	{
		const auto tag = "$skinnedbox." + Uuid::ToString(Uuid::New());
		auto model = Geometry::Cube::MakeIndependentTextured();
		model.SetNormalsIndependentFlat();
		model.Transform(XMMatrixScaling(scale, scale, scale));
		SetPos(position);

		pVertices = Bind::VertexBuffer::Resolve(gfx, tag, model.vertices);
		pIndices = Bind::IndexBuffer::Resolve(gfx, tag, model.indices);
		pTopology = Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		{
			Technique shade("Shade");
			{
				Step only(0);

				only.AddBindable(Bind::Texture::Resolve(gfx, "Images\\brickwall.jpg"));
				only.AddBindable(Bind::Sampler::Resolve(gfx));

				auto pvs = Bind::VertexShader::Resolve(gfx, "TexturedPhongVS.cso");
				auto pvsbc = pvs->GetBytecode();
				only.AddBindable(std::move(pvs));

				only.AddBindable(Bind::PixelShader::Resolve(gfx, "TexturedPhongPS.cso"));

				Dcb::RawLayout lay;
				lay.Add<Dcb::Float>("specularIntensity");
				lay.Add<Dcb::Float>("specularPower");
				auto buf = Dcb::Buffer(std::move(lay));
				buf["specularIntensity"] = 0.1f;
				buf["specularPower"] = 20.0f;
				only.AddBindable(std::make_shared<Bind::CachingDynamicPixelCBuf>(gfx, buf, 1u));

				only.AddBindable(std::make_unique<Bind::InputLayout>(gfx, model.vertices.GetLayout(), pvsbc));
				only.AddBindable(std::make_shared<Bind::TransformCBuf>(gfx));

				shade.AddStep(std::move(only));
			}
			AddTechnique(std::move(shade));
		}

		Technique outline("Outline");
		{
			Step mask(1);
			{
				auto pvs = Bind::VertexShader::Resolve(gfx, "SolidVS.cso");
				auto pvsbc = pvs->GetBytecode();
				mask.AddBindable(std::move(pvs));

				// TODO: better sub-layout generation tech for future consideration maybe
				mask.AddBindable(Bind::InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

				mask.AddBindable(std::make_shared<Bind::TransformCBuf>(gfx));

				// TODO: might need to specify rasterizer when doubled-sided models start being used
				outline.AddStep(std::move(mask));
			}

			Step draw(2);
			{
				// these can be pass-constant (tricky due to layout issues)
				auto pvs = Bind::VertexShader::Resolve(gfx, "SolidVS.cso");
				auto pvsbc = pvs->GetBytecode();
				draw.AddBindable(std::move(pvs));

				// this can be pass-constant
				draw.AddBindable(Bind::PixelShader::Resolve(gfx, "SolidPS.cso"));

				Dcb::RawLayout lay;
				lay.Add<Dcb::Float4>("color");
				auto buf = Dcb::Buffer(std::move(lay));
				buf["color"] = XMFLOAT4{1.0f, 0.4f, 0.4f, 1.0f};
				draw.AddBindable(std::make_shared<Bind::CachingDynamicPixelCBuf>(gfx, buf, 1u));

				// TODO: better sub-layout generation tech for future consideration maybe
				draw.AddBindable(Bind::InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));

				// quick and dirty... nicer solution maybe takes a lambda... we'll see :)
				class TransformCBufScaling : public Bind::TransformCBuf
				{
				public:
					TransformCBufScaling(Graphics& gfx, float scale = 1.04)
						: TransformCBuf(gfx),
						  buf(MakeLayout())
					{
						buf["scale"] = scale;
					}

					void Accept(TechniqueProbe& probe) override
					{
						probe.VisitBuffer(buf);
					}

					void Bind(Graphics& gfx) noexcept override
					{
						const float scale = buf["scale"];
						const auto scaleMatrix = XMMatrixScaling(scale, scale, scale);
						auto xf = GetTransforms(gfx);
						xf.modelView = xf.modelView * scaleMatrix;
						xf.modelViewProj = xf.modelViewProj * scaleMatrix;
						UpdateBindImpl(gfx, xf);
					}

				private:
					static Dcb::RawLayout MakeLayout()
					{
						Dcb::RawLayout layout;
						layout.Add<Dcb::Float>("scale");
						return layout;
					}

					Dcb::Buffer buf;
				};
				draw.AddBindable(std::make_shared<TransformCBufScaling>(gfx));

				// TODO: might need to specify rasterizer when doubled-sided models start being used
				outline.AddStep(std::move(draw));
			}
			AddTechnique(outline);
		}
	}

	bool SkinnedBox::SpawnControlWindow() noexcept
	{
		bool dirty = false;
		bool open = true;
		if (ImGui::Begin(("Skinned Box##" + std::to_string(id)).c_str(), &open))
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
					ImGui::Checkbox(("Tech Active##"s + std::to_string(techId)).c_str(), &active);
					pTech->SetActive(active);
				}

				bool OnVisitBuffer(Dcb::Buffer& buf) override
				{
					float dirty = false;
					const auto dcheck = [&dirty](bool changed) { dirty = dirty || changed; };
					auto tag = [tagScratch = std::string{}, tagString = "##" + std::to_string(bufId)]
					(const char* label) mutable
					{
						tagScratch = label + tagString;
						return tagScratch.c_str();
					};

					if (auto v = buf["scale"]; v.Exists())
					{
						dcheck(ImGui::SliderFloat(tag("Scale"), &v, 1.0f, 2.0f, "%.3f"));
					}
					if (auto v = buf["color"]; v.Exists())
					{
						dcheck(ImGui::ColorPicker4(tag("Color"), reinterpret_cast<float*>(&static_cast<XMFLOAT4&>(v))));
					}
					if (auto v = buf["specularIntensity"]; v.Exists())
					{
						dcheck(ImGui::SliderFloat(tag("Spec. Intens."), &v, 0.0f, 1.0f));
					}
					if (auto v = buf["specularPower"]; v.Exists())
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
