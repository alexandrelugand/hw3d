#include "stdafx.h"
#include "MeshTechniqueProbe.h"

namespace Probes
{
	void MeshTechniqueProbe::OnSetTechnique()
	{
		using namespace std::string_literals;
		ImGui::TextColored({0.4f, 1.0f, 0.6f, 1.0f}, pTech->GetName().c_str());
		bool active = pTech->IsActive();
		ImGui::Checkbox(("Tech Active##"s + std::to_string(techIdx)).c_str(), &active);
		pTech->SetActive(active);
	}

	bool MeshTechniqueProbe::OnVisitBuffer(Dcb::Buffer& buf)
	{
		namespace dx = DirectX;
		float dirty = false;
		const auto dcheck = [&dirty](bool changed) { dirty = dirty || changed; };
		auto tag = [tagScratch = std::string{}, tagString = "##" + std::to_string(bufIdx)]
		(const char* label) mutable
		{
			tagScratch = label + tagString;
			return tagScratch.c_str();
		};

		if (auto v = buf["scale"]; v.Exists())
		{
			dcheck(ImGui::SliderFloat(tag("Scale"), &v, 1.0f, 2.0f, "%.3f"));
		}
		if (auto v = buf["offset"]; v.Exists())
		{
			dcheck(ImGui::SliderFloat(tag("offset"), &v, 0.0f, 1.0f, "%.3f"));
		}
		if (auto v = buf["materialColor"]; v.Exists())
		{
			dcheck(ImGui::ColorPicker3(tag("Color"), reinterpret_cast<float*>(&static_cast<XMFLOAT3&>(v))));
		}
		if (auto v = buf["specularColor"]; v.Exists())
		{
			dcheck(ImGui::ColorPicker3(tag("Spec. Color"), reinterpret_cast<float*>(&static_cast<XMFLOAT3&>(v))));
		}
		if (auto v = buf["specularGloss"]; v.Exists())
		{
			dcheck(ImGui::SliderFloat(tag("Glossiness"), &v, 1.0f, 100.0f, "%.1f"));
		}
		if (auto v = buf["specularWeight"]; v.Exists())
		{
			dcheck(ImGui::SliderFloat(tag("Spec. Weight"), &v, 0.0f, 2.0f));
		}
		if (auto v = buf["useSpecularMap"]; v.Exists())
		{
			dcheck(ImGui::Checkbox(tag("Spec. Map Enable"), &v));
		}
		if (auto v = buf["useNormalMap"]; v.Exists())
		{
			dcheck(ImGui::Checkbox(tag("Normal Map Enable"), &v));
		}
		if (auto v = buf["normalMapWeight"]; v.Exists())
		{
			dcheck(ImGui::SliderFloat(tag("Normal Map Weight"), &v, 0.0f, 2.0f));
		}
		return dirty;
	}
}
