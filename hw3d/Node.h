#pragma once

namespace Entities
{
	class Node
	{
		friend class Model;
		friend class Windowing::ModelWindow;

	public:
		Node(const std::string& name, std::vector<Mesh*> meshPtrs, const XMMATRIX& transform_in) noexcpt;

		void Draw(Graphics& gfx, FXMMATRIX accumulatedTransform) const noexcpt;
		void SetAppliedTransform(FXMMATRIX transform) noexcpt;

	private:
		void AddChild(std::unique_ptr<Node> pChild) noexcpt;
		void ShowTree(int& nodeIndex, std::optional<int>& selectedIndex, Node*& pSelectedNode) const noexcpt;
		void ResetNode() noexcpt;

		std::string name;
		std::vector<Mesh*> meshPtrs;
		std::vector<std::unique_ptr<Node>> childPtrs;
		XMFLOAT4X4 transform;
		XMFLOAT4X4 appliedTransform;
	};
}
