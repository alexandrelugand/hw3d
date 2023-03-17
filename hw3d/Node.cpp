#include "stdafx.h"
#include "Node.h"

namespace Entities
{
	Node::Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const XMMATRIX& transform_in) noexcpt
		: name(name), id(id), meshPtrs(meshPtrs)
	{
		XMStoreFloat4x4(&transform, transform_in);
		XMStoreFloat4x4(&appliedTransform, XMMatrixIdentity());
	}

	void Node::Draw(Graphics& gfx, FXMMATRIX accumulatedTransform) const noexcpt
	{
		const auto built = XMLoadFloat4x4(&appliedTransform) *
			XMLoadFloat4x4(&transform) *
			accumulatedTransform;

		for (const auto pm : meshPtrs)
		{
			pm->Draw(gfx, built);
		}

		for (const auto& pc : childPtrs)
		{
			pc->Draw(gfx, built);
		}
	}

	void Node::SetAppliedTransform(FXMMATRIX transform) noexcpt
	{
		XMStoreFloat4x4(&appliedTransform, transform);
	}

	void Node::ShowTree(Node*& pSelectedNode) const noexcpt
	{
		// if there is no selected node, set selectedId to an impossible value
		const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->GetId();

		// build up flags for current node
		const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_OpenOnDoubleClick
			| ((GetId() == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
			| ((childPtrs.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);

		// render this node
		const auto expanded = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<intptr_t>(GetId())), node_flags, name.c_str());

		// detecting / setting selected node
		if (ImGui::IsItemClicked())
		{
			pSelectedNode = const_cast<Node*>(this);
		}

		// if tree node expanded, recursively render all children
		if (expanded)
		{
			for (const auto& pChild : childPtrs)
			{
				pChild->ShowTree(pSelectedNode);
			}
			ImGui::TreePop();
		}
	}

	void Node::ResetNode() noexcpt
	{
		XMStoreFloat4x4(&appliedTransform, XMMatrixIdentity());
		for (const auto& pChild : childPtrs)
		{
			pChild->ResetNode();
		}
	}

	void Node::AddChild(std::unique_ptr<Node> pChild) noexcpt
	{
		assert(pChild);
		childPtrs.push_back(std::move(pChild));
	}
}
