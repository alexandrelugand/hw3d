#include "stdafx.h"
#include "ModelWindow.h"

namespace Windowing
{
	void ModelWindow::Show(Graphics& gfx, const char* windowName, Entities::Model& model, Entities::Node& root) noexcpt
	{
		/*
		// window name defaults to "Model
		windowName = windowName ? windowName : "Model";

		// need an int to track node indices and selected node
		if (ImGui::Begin(windowName))
		{
			ImGui::Columns(2, nullptr, true);
			if (ImGui::Button("Reset all"))
			{
				root.ResetNode();
				transforms.clear();
				pSelectedNode = nullptr;
			}

			root.ShowTree(pSelectedNode);

			ImGui::NextColumn();

			if (pSelectedNode != nullptr)
			{
				const auto id = pSelectedNode->GetId();
				auto i = transforms.find(id);
				if (i == transforms.end())
				{
					const auto& applied = pSelectedNode->GetAppliedTransform();
					const auto angles = ExtractEulerAngles(applied);
					const auto translation = ExtractTranslation(applied);
					TransformParameters tp;
					tp.roll = angles.z;
					tp.pitch = angles.x;
					tp.yaw = angles.y;
					tp.x = translation.x;
					tp.y = translation.y;
					tp.z = translation.z;
					auto pMatConst = pSelectedNode->GetMaterialConstants();
					auto buf = pMatConst != nullptr ? std::optional<Dcb::Buffer>{*pMatConst} : std::optional<Dcb::Buffer>{};
					std::tie(i, std::ignore) = transforms.insert({id, {tp, false, std::move(buf), false}});
				}

				// link imgui ctrl to our cached transform params
				auto& transform = i->second.tranformParams;
				// dirty check
				auto& dirtyTransform = i->second.transformParamsDirty;
				const auto dcheckTransform = [&dirtyTransform](bool changed) { dirtyTransform = dirtyTransform || changed; };

				// widgets
				ImGui::Text("Orientation");
				dcheckTransform(ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f));
				dcheckTransform(ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f));
				dcheckTransform(ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f));

				ImGui::Text("Position");
				dcheckTransform(ImGui::SliderFloat("X", &transform.x, -20.0f, 20.0f));
				dcheckTransform(ImGui::SliderFloat("Y", &transform.y, -20.0f, 20.0f));
				dcheckTransform(ImGui::SliderFloat("Z", &transform.z, -20.0f, 20.0f));

				// link imgui ctrl to our cached material params
				if (i->second.materialCbuf)
				{
					auto& mat = *i->second.materialCbuf;
					// dirty check
					auto& dirtyMat = i->second.materialCbufDirty;
					const auto dcheckMat = [&dirtyMat](bool changed) { dirtyMat = dirtyMat || changed; };
					// widgets
					ImGui::Text("Material");
					if (auto v = mat["normalMapEnabled"]; v.Exists())
					{
						dcheckMat(ImGui::Checkbox("Norm Map", &v));
					}
					if (auto v = mat["specularMapEnabled"]; v.Exists())
					{
						dcheckMat(ImGui::Checkbox("Spec Map", &v));
					}
					if (auto v = mat["hasGlossMap"]; v.Exists())
					{
						dcheckMat(ImGui::Checkbox("Gloss Map", &v));
					}
					if (auto v = mat["materialColor"]; v.Exists())
					{
						dcheckMat(ImGui::ColorPicker3("Diff Color", reinterpret_cast<float*>(&static_cast<XMFLOAT3&>(v))));
					}
					if (auto v = mat["specularPower"]; v.Exists())
					{
						dcheckMat(ImGui::SliderFloat("Spec Power", &v, 0.0f, 100.0f, "%.1f"));
					}
					if (auto v = mat["specularColor"]; v.Exists())
					{
						dcheckMat(ImGui::ColorPicker3("Spec Color", reinterpret_cast<float*>(&static_cast<XMFLOAT3&>(v))));
					}
					if (auto v = mat["specularMapWeight"]; v.Exists())
					{
						dcheckMat(ImGui::SliderFloat("Spec Weight", &v, 0.0f, 4.0f));
					}
					if (auto v = mat["specularIntensity"]; v.Exists())
					{
						dcheckMat(ImGui::SliderFloat("Spec Intens", &v, 0.0f, 1.0f));
					}
				}

				if (ImGui::Button("Reset"))
				{
					transform = {};
					i->second.transformParamsDirty = true;
				}
			}
		}
		ImGui::End();
		*/
	}

	void ModelWindow::ApplyParameters() noexcept(!true)
	{
		/*
		if (TransformDirty())
		{
			pSelectedNode->SetAppliedTransform(GetTransform());
			ResetTransformDirty();
		}
		if (MaterialDirty())
		{
			pSelectedNode->SetMaterialConstants(GetMaterial());
			ResetMaterialDirty();
		}
		*/
	}

	XMMATRIX ModelWindow::GetTransform() const noexcpt
	{
		assert(pSelectedNode != nullptr);
		const auto& transform = transforms.at(pSelectedNode->GetId()).tranformParams;
		return
			XMMatrixRotationRollPitchYaw(transform.roll, transform.pitch, transform.yaw) *
			XMMatrixTranslation(transform.x, transform.y, transform.z);
	}

	const Dcb::Buffer& ModelWindow::GetMaterial() const noexcpt
	{
		assert(pSelectedNode != nullptr);
		const auto& mat = transforms.at(pSelectedNode->GetId()).materialCbuf;
		assert(mat);
		return *mat;
	}

	bool ModelWindow::TransformDirty() const noexcpt
	{
		return pSelectedNode && transforms.at(pSelectedNode->GetId()).transformParamsDirty;
	}

	void ModelWindow::ResetTransformDirty() noexcpt
	{
		transforms.at(pSelectedNode->GetId()).transformParamsDirty = false;
	}

	bool ModelWindow::MaterialDirty() const noexcpt
	{
		return pSelectedNode && transforms.at(pSelectedNode->GetId()).materialCbufDirty;
	}

	void ModelWindow::ResetMaterialDirty() noexcpt
	{
		transforms.at(pSelectedNode->GetId()).materialCbufDirty = false;
	}

	bool ModelWindow::IsDirty() const noexcpt
	{
		return TransformDirty() || MaterialDirty();
	}

	//Entities::Node* ModelWindow::GetSelectedNode() const noexcept
	//{
	//	return pSelectedNode;
	//}
}
