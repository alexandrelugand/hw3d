#include "stdafx.h"
#include "Sheet.h"
#include "Plane.h"

namespace Draw
{
	Sheet::Sheet(Graphics& gfx, XMFLOAT3 material)
		: DrawableObject(gfx)
	{
		const auto tag = "$sheet." + Uuid::ToString(Uuid::New());
		auto model = Geometry::Plane::Make(1, 1);
		materialConstants.color = material;

		AddBind(Bind::VertexBuffer::Resolve(gfx, tag, model.vbd));
		AddBind(Bind::IndexBuffer::Resolve(gfx, tag, model.indices));

		AddBind(Bind::Texture::Resolve(gfx, "images\\alex.png"));
		AddBind(Bind::Sampler::Resolve(gfx));

		const auto pvs = Bind::VertexShader::Resolve(gfx, "TextureVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddBind(std::move(pvs));

		AddBind(Bind::PixelShader::Resolve(gfx, "TexturePS.cso"));

		AddBind(Bind::InputLayout::Resolve(gfx, model.vbd.GetLayout(), pvsbc));
		AddBind(Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		AddBind(Bind::Cull::Resolve(gfx, None));

		AddBind(std::make_unique<Bind::TransformCBuf>(gfx, *this));
	}

	bool Sheet::SpawnControlWindow() noexcept
	{
		bool dirty = false;
		bool open = true;
		if (ImGui::Begin("Sheet", &open))
		{
			ImGui::Text("Material Properties");
			const auto cd = ImGui::ColorEdit3("Material Color", &materialConstants.color.x);
			const auto sid = ImGui::SliderFloat("Specular Intensity", &materialConstants.specularIntensity, 0.05f, 4.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
			const auto spd = ImGui::SliderFloat("Specular Power", &materialConstants.specularPower, 1.0f, 200.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
			dirty = cd || sid || spd;

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

	void Sheet::SyncMaterial() noexcpt
	{
		const auto pConstPS = QueryBindable<MaterialCbuf>();
		assert(pConstPS != nullptr);
		pConstPS->Update(gfx, materialConstants);
	}
}
