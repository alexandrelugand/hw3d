#include "stdafx.h"
#include "SkinnedBox.h"
#include "Cube.h"

namespace Draw
{
	SkinnedBox::SkinnedBox(Graphics& gfx)
		: DrawableObject(gfx)
	{
		const auto tag = "$skinnedbox." + Uuid::ToString(Uuid::New());
		const auto model = Geometry::Cube::MakeIndependentTextured();

		AddBind(Bind::VertexBuffer::Resolve(gfx, tag, model.vbd));
		AddBind(Bind::IndexBuffer::Resolve(gfx, tag, model.indices));

		auto pvs = Bind::VertexShader::Resolve(gfx, "TexturedPhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddBind(std::move(pvs));

		AddBind(Bind::PixelShader::Resolve(gfx, "TexturedPhongPS.cso"));

		AddBind(std::make_unique<Bind::InputLayout>(gfx, model.vbd.GetLayout(), pvsbc));
		AddBind(std::make_unique<Bind::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		AddBind(std::make_unique<MaterialCbuf>(gfx, materialConstants, 1u));

		AddBind(Bind::Texture::Resolve(gfx, "images\\alex.png"));
		AddBind(Bind::Sampler::Resolve(gfx));

		AddBind(std::make_shared<Bind::TransformCBuf>(gfx, *this));
	}

	bool SkinnedBox::SpawnControlWindow() noexcept
	{
		bool dirty = false;
		bool open = true;
		if (ImGui::Begin("Skinned Box", &open))
		{
			ImGui::Text("Material Properties");
			const auto sid = ImGui::SliderFloat("Specular Intensity", &materialConstants.specularIntensity, 0.05f, 4.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
			const auto spd = ImGui::SliderFloat("Specular Power", &materialConstants.specularPower, 1.0f, 200.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
			dirty = sid || spd;

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

		if (dirty)
		{
			SyncMaterial();
		}

		return open;
	}

	void SkinnedBox::SyncMaterial() noexcpt
	{
		const auto pConstPS = QueryBindable<MaterialCbuf>();
		assert(pConstPS != nullptr);
		pConstPS->Update(gfx, materialConstants);
	}
}
