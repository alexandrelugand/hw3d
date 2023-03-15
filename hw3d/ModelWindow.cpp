#include "stdafx.h"
#include "ModelWindow.h"

namespace Windowing
{
	void ModelWindow::Show(const char* windowName, Entities::Model& model, Entities::Node& root) noexcpt
	{
		// window name defaults to "Model
		windowName = windowName ? windowName : "Model";

		// need an int to track node indices and selected node
		if (ImGui::Begin(windowName))
		{
			ImGui::Columns(2, nullptr, true);
			if (ImGui::Button("Reset all"))
			{
				root.ResetNode();
				for (auto& val : transforms | std::views::values)
				{
					val = {};
				}
				pSelectedNode = nullptr;
			}

			root.ShowTree(pSelectedNode);

			ImGui::NextColumn();
			if (pSelectedNode != nullptr)
			{
				auto& transform = transforms[pSelectedNode->GetId()];
				ImGui::Text("Orientation");
				ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f);
				ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f);
				ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f);

				ImGui::Text("Position");
				ImGui::SliderFloat("X", &transform.x, -20.0f, 20.0f);
				ImGui::SliderFloat("Y", &transform.y, -20.0f, 20.0f);
				ImGui::SliderFloat("Z", &transform.z, -20.0f, 20.0f);

				if (ImGui::Button("Reset"))
					transform = {};
			}
		}
		ImGui::End();
	}

	XMMATRIX ModelWindow::GetTransform() const noexcpt
	{
		assert(pSelectedNode != nullptr);
		const auto& transform = transforms.at(pSelectedNode->GetId());
		return XMMatrixRotationRollPitchYaw(transform.pitch, transform.yaw, transform.roll) * XMMatrixTranslation(transform.x, -transform.y, -transform.z);
	}
}
