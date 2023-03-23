#include "stdafx.h"
#include "Box.h"
#include "Cube.h"

namespace Draw
{
	Box::Box(Graphics& gfx, XMFLOAT3 material, const float scale, const XMFLOAT3& position)
		: DrawableObject(gfx, scale, position)
	{
		const auto tag = "$box." + Uuid::ToString(Uuid::New());

		auto model = Geometry::Cube::Make();
		model.SetNormalsIndependentFlat();

		pVertices = Bind::VertexBuffer::Resolve(gfx, tag, model.vertices);
		pIndices = Bind::IndexBuffer::Resolve(gfx, tag, model.indices);
		pTopology = Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		{
			Technique shade("Shade");
			{
				Step only(0);

				auto pvs = Bind::VertexShader::Resolve(gfx, "BlendedPhongVS.cso");
				auto pvsbc = pvs->GetBytecode();
				only.AddBindable(std::move(pvs));

				only.AddBindable(Bind::PixelShader::Resolve(gfx, "BlendedPhongPS.cso"));

				Dcb::RawLayout lay;
				lay.Add<Dcb::Float3>("color");
				lay.Add<Dcb::Float>("specularIntensity");
				lay.Add<Dcb::Float>("specularPower");
				auto buf = Dcb::Buffer(std::move(lay));
				buf["color"] = material;
				buf["specularIntensity"] = 0.1f;
				buf["specularPower"] = 20.0f;
				only.AddBindable(std::make_shared<Bind::CachingDynamicPixelCBuf>(gfx, buf, 1u));

				only.AddBindable(std::make_unique<Bind::InputLayout>(gfx, model.vertices.GetLayout(), pvsbc));
				only.AddBindable(std::make_shared<Bind::TransformCBuf>(gfx));

				shade.AddStep(std::move(only));
			}
			AddTechnique(std::move(shade));
		}
	}

	bool Box::SpawnControlWindow() noexcept
	{
		bool open = true;
		if (ImGui::Begin(("Box##"s + std::to_string(id)).c_str(), &open))
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

			if (ImGui::Button("Reset"))
				Reset();
		}
		ImGui::End();

		return open;
	}
}
