#include "stdafx.h"
#include "PointLight.h"

PointLight::PointLight(Graphics& gfx, float radius, XMFLOAT3 color)
	: mesh(gfx, radius, color),
	  cbuf(gfx)
{
	Reset();
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &cbData.pos.x, -60.0f, 60.0f, "%.1lf");
		ImGui::SliderFloat("Y", &cbData.pos.y, -60.0f, 60.0f, "%.1lf");
		ImGui::SliderFloat("Z", &cbData.pos.z, -60.0f, 60.0f, "%.1lf");

		ImGui::Text("Intensity/Color");
		ImGui::ColorEdit3("Diffuse Color", &cbData.diffuseColor.x);
		ImGui::ColorEdit3("Ambient", &cbData.ambient.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &cbData.attConst, 0.05f, 10.0f, "%.2lf", ImGuiSliderFlags_NoRoundToFormat);
		ImGui::SliderFloat("Linear", &cbData.attLin, 0.0001f, 4.0f, "%.4lf", ImGuiSliderFlags_NoRoundToFormat);
		ImGui::SliderFloat("Quadratic", &cbData.attQuad, 0.0000001f, 1.0f, "%.7lf", ImGuiSliderFlags_NoRoundToFormat);

		if (ImGui::Button("Reset"))
			Reset();
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	cbData = {
		{0.0f, 0.0f, 0.0f},
		{0.05f, 0.05f, 0.05f},
		{1.0f, 1.0f, 1.0f},
		1.0f,
		1.0f,
		0.045f,
		0.0075f
	};
}

void PointLight::Draw(Graphics& gfx) const noexcpt
{
	mesh.SetPos(cbData.pos);
	mesh.SetColor(cbData.diffuseColor);
	mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx, FXMMATRIX view) const noexcept
{
	auto dataCopy = cbData;
	const auto pos = XMLoadFloat3(&dataCopy.pos);
	XMStoreFloat3(&dataCopy.pos, XMVector3Transform(pos, view));
	cbuf.Update(gfx, dataCopy);
	cbuf.Bind(gfx);
}
