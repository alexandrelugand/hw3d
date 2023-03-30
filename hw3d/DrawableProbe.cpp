#include "stdafx.h"
#include "DrawableProbe.h"

namespace Probes
{
	void DrawableProbe::SpawnControl(Draw::DrawableObject& draw) const noexcept
	{
		ImGui::Begin(draw.GetSysName().c_str());
		{
			bool dirty = false;
			const auto dcheck = [&dirty](bool changed) { dirty = dirty || changed; };
			auto pos = draw.GetPosition();
			auto rot = draw.GetRotation();
			ImGui::TextColored({0.4f, 1.0f, 0.6f, 1.0f}, "Translation");
			dcheck(ImGui::SliderFloat("X", &pos.x, -60.f, 60.f));
			dcheck(ImGui::SliderFloat("Y", &pos.y, -60.f, 60.f));
			dcheck(ImGui::SliderFloat("Z", &pos.z, -60.f, 60.f));
			ImGui::TextColored({0.4f, 1.0f, 0.6f, 1.0f}, "Orientation");
			dcheck(ImGui::SliderAngle("X-rot", &rot.x, -180.0f, 180.0f));
			dcheck(ImGui::SliderAngle("Y-rot", &rot.y, -180.0f, 180.0f));
			dcheck(ImGui::SliderAngle("Z-rot", &rot.z, -180.0f, 180.0f));
			if (dirty)
			{
				draw.SetPosition(pos);
				draw.SetRotation(rot);
			}

			MeshTechniqueProbe probe;
			draw.Accept(probe);
		}
		ImGui::End();
	}
}
