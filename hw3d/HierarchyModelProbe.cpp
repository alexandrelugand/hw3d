#include "stdafx.h"
#include "HierarchyModelProbe.h"

namespace Probes
{
	void HierarchyModelProbe::SpawnWindow(const Entities::Model& model)
	{
		ImGui::Begin(model.GetName().c_str());
		ImGui::Columns(2, nullptr, true);
		model.Accept(*this);

		ImGui::NextColumn();
		if (pSelectedNode != nullptr)
		{
			bool dirty = false;
			const auto dcheck = [&dirty](bool changed) { dirty = dirty || changed; };
			auto& tf = ResolveTransform();
			ImGui::TextColored({0.4f, 1.0f, 0.6f, 1.0f}, "Translation");
			dcheck(ImGui::SliderFloat("X", &tf.x, -60.f, 60.f));
			dcheck(ImGui::SliderFloat("Y", &tf.y, -60.f, 60.f));
			dcheck(ImGui::SliderFloat("Z", &tf.z, -60.f, 60.f));
			ImGui::TextColored({0.4f, 1.0f, 0.6f, 1.0f}, "Orientation");
			dcheck(ImGui::SliderAngle("X-rot", &tf.xRot, -180.0f, 180.0f));
			dcheck(ImGui::SliderAngle("Y-rot", &tf.yRot, -180.0f, 180.0f));
			dcheck(ImGui::SliderAngle("Z-rot", &tf.zRot, -180.0f, 180.0f));
			if (dirty)
			{
				pSelectedNode->SetAppliedTransform(
					XMMatrixRotationX(tf.xRot) *
					XMMatrixRotationY(tf.yRot) *
					XMMatrixRotationZ(tf.zRot) *
					XMMatrixTranslation(tf.x, tf.y, tf.z)
				);
			}

			MeshTechniqueProbe probe;
			pSelectedNode->Accept(probe);
		}
		ImGui::End();
	}

	bool HierarchyModelProbe::PushNode(Entities::Node& node)
	{
		// if there is no selected node, set selectedId to an impossible value
		const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->GetId();

		// build up flags for current node
		const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
			| ((node.GetId() == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
			| (node.HasChildren() ? 0 : ImGuiTreeNodeFlags_Leaf);

		// render this node
		const auto expanded = ImGui::TreeNodeEx(
			(void*)static_cast<intptr_t>(node.GetId()),
			node_flags, node.GetName().c_str()
		);

		// processing for selecting node
		if (ImGui::IsItemClicked())
			pSelectedNode = &node;

		// signal if children should also be recursed
		return expanded;
	}

	void HierarchyModelProbe::PopNode(Entities::Node& node)
	{
		ImGui::TreePop();
	}

	TransformParameters& HierarchyModelProbe::ResolveTransform() noexcept
	{
		const auto id = pSelectedNode->GetId();
		const auto i = transformParams.find(id);
		if (i == transformParams.end())
			return LoadTransform(id);

		return i->second;
	}

	TransformParameters& HierarchyModelProbe::LoadTransform(int id) noexcept
	{
		const auto& applied = pSelectedNode->GetAppliedTransform();
		const auto angles = ExtractEulerAngles(applied);
		const auto translation = ExtractTranslation(applied);

		TransformParameters tp;
		tp.zRot = angles.z;
		tp.xRot = angles.x;
		tp.yRot = angles.y;
		tp.x = translation.x;
		tp.y = translation.y;
		tp.z = translation.z;
		return transformParams.insert({id, {tp}}).first->second;
	}
}
