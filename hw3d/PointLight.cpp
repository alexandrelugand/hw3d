#include "stdafx.h"
#include "PointLight.h"

namespace Entities
{
	PointLight::PointLight(Graphics& gfx, XMFLOAT3 pos, float pitch, float yaw, float radius, XMFLOAT3 color)
		: mesh(gfx, radius, color),
		  cbuf(gfx)
	{
		home = {
			pos,
			{0.05f, 0.05f, 0.05f},
			{1.0f, 1.0f, 1.0f},
			1.0f,
			1.0f,
			0.045f,
			0.0075f,
		};
		Reset();
		pCamera = std::make_shared<Camera>(gfx, "Light", cbData.pos, pitch, yaw, true);
	}

	void PointLight::SpawnControlWindow() noexcept
	{
		if (ImGui::Begin("Light"))
		{
			bool dirtyPos = false;
			const auto dcheck = [&dirtyPos](bool dirty) { dirtyPos = dirtyPos || dirty; };

			ImGui::Text("Position");
			dcheck(ImGui::SliderFloat("X", &cbData.pos.x, -100.0f, 100.0f, "%.1lf"));
			dcheck(ImGui::SliderFloat("Y", &cbData.pos.y, -100.0f, 100.0f, "%.1lf"));
			dcheck(ImGui::SliderFloat("Z", &cbData.pos.z, -100.0f, 100.0f, "%.1lf"));
			if (dirtyPos)
				pCamera->SetPos(cbData.pos);

			ImGui::Text("Intensity/Color");
			ImGui::ColorEdit3("Diffuse Color", &cbData.diffuseColor.x);
			ImGui::ColorEdit3("Ambient", &cbData.ambient.x);

			ImGui::Text("Falloff");
			ImGui::SliderFloat("Constant", &cbData.attConst, 0.05f, 10.0f, "%.2lf", ImGuiSliderFlags_NoRoundToFormat);
			ImGui::SliderFloat("Linear", &cbData.attLin, 0.0001f, 4.0f, "%.4lf", ImGuiSliderFlags_NoRoundToFormat);
			ImGui::SliderFloat("Quadratic", &cbData.attQuad, 0.0000001f, 0.1f, "%.7lf", ImGuiSliderFlags_NoRoundToFormat);

			if (ImGui::Button("Reset"))
				Reset();
		}
		ImGui::End();
	}

	void PointLight::Reset() noexcept
	{
		cbData = home;
	}

	void PointLight::Submit(size_t channelFilter) const noexcpt
	{
		mesh.SetPos(cbData.pos);
		mesh.SetColor(cbData.diffuseColor);
		mesh.Submit(channelFilter);
	}

	void PointLight::Bind(Graphics& gfx, FXMMATRIX view) const noexcept
	{
		auto dataCopy = cbData;
		const auto pos = XMLoadFloat3(&dataCopy.pos);
		XMStoreFloat3(&dataCopy.pos, XMVector3Transform(pos, view));
		cbuf.Update(gfx, dataCopy);
		cbuf.Bind(gfx);
	}

	void PointLight::LinkTechniques(Rgph::RenderGraph& rg) const
	{
		mesh.LinkTechniques(rg);
	}

	std::shared_ptr<Camera> PointLight::ShareCamera() const noexcept
	{
		return pCamera;
	}
}
