#include "stdafx.h"
#include "Pyramid.h"
#include "Cone.h"

namespace Draw
{
	Pyramid::Pyramid(Graphics& gfx, XMFLOAT3 material)
		: DrawableObject(gfx)
	{
		const auto tag = "$pyramid." + Uuid::ToString(Uuid::New());
		const auto model = Geometry::Cone::MakeTesselatedIndependentNormals(24);
		materialConstants.color = material;

		AddBind(Bind::VertexBuffer::Resolve(gfx, tag, model.vbd));
		AddBind(Bind::IndexBuffer::Resolve(gfx, tag, model.indices));

		const auto pvs = Bind::VertexShader::Resolve(gfx, "BlendedPhongVS.cso");
		const auto pvsbc = pvs->GetBytecode();
		AddBind(std::move(pvs));

		AddBind(Bind::PixelShader::Resolve(gfx, "BlendedPhongPS.cso"));

		AddBind(Bind::InputLayout::Resolve(gfx, model.vbd.GetLayout(), pvsbc));
		AddBind(Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		AddBind(std::make_shared<Bind::TransformCBuf>(gfx, *this));
		AddBind(std::make_shared<MaterialCbuf>(gfx, materialConstants, 1u));
	}

	bool Pyramid::SpawnControlWindow() noexcept
	{
		bool dirty = false;
		bool open = true;
		if (ImGui::Begin("Pyramid", &open))
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

	void Pyramid::SyncMaterial() noexcpt
	{
		const auto pConstPS = QueryBindable<MaterialCbuf>();
		assert(pConstPS != nullptr);
		pConstPS->Update(gfx, materialConstants);
	}
}
