#include "stdafx.h"
#include "Sheet.h"
#include "Plane.h"
#include "TransformAllCBuf.h"

namespace Draw
{
	Sheet::Sheet(Graphics& gfx, float size)
		: DrawableObject(gfx)
	{
		const auto tag = "$sheet." + Uuid::ToString(Uuid::New());
		auto model = Geometry::Plane::Make(1, 1);
		model.Transform(XMMatrixScaling(size, size, 1.0f));

		AddBind(Bind::VertexBuffer::Resolve(gfx, tag, model.vbd));
		AddBind(Bind::IndexBuffer::Resolve(gfx, tag, model.indices));

		AddBind(Bind::Texture::Resolve(gfx, "images\\brickwall.jpg"));
		AddBind(Bind::Texture::Resolve(gfx, "images\\brickwall_normal.jpg", 1u));

		const auto pvs = Bind::VertexShader::Resolve(gfx, "PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddBind(std::move(pvs));

		AddBind(Bind::PixelShader::Resolve(gfx, "PhongPSNormalMap.cso"));
		AddBind(Bind::PixelConstantBuffer<BumpMappingConstant>::Resolve(gfx, pmc, 1u));

		AddBind(Bind::InputLayout::Resolve(gfx, model.vbd.GetLayout(), pvsbc));
		AddBind(Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		AddBind(Bind::Cull::Resolve(gfx, None));

		AddBind(std::make_shared<Bind::TransformAllCBuf>(gfx, *this, 0u, 2u));
	}

	bool Sheet::SpawnControlWindow() noexcept
	{
		bool dirty = false;
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

			const bool changed0 = ImGui::SliderFloat("Spec. Int.", &pmc.specularIntensity, 0.0f, 1.0f);
			const bool changed1 = ImGui::SliderFloat("Spec. Power", &pmc.specularPower, 0.0f, 100.0f);
			bool checkState = pmc.normalMappingEnabled == TRUE;
			const bool changed2 = ImGui::Checkbox("Enable Normal Map", &checkState);
			pmc.normalMappingEnabled = checkState ? TRUE : FALSE;
			if (changed0 || changed1 || changed2)
			{
				dirty = true;
			}

			if (ImGui::Button("Reset"))
				Reset();
		}
		ImGui::End();

		if (dirty)
		{
			SyncConstPS();
		}

		return open;
	}

	void Sheet::SyncConstPS() noexcpt
	{
		const auto pConstPS = QueryBindable<ConstPS>();
		assert(pConstPS != nullptr);
		pConstPS->Update(gfx, pmc);
	}
}
