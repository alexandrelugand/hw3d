#pragma once

namespace Entities
{
	class Node
	{
		friend class Model;

	public:
		Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const XMMATRIX& transform_in) noexcpt;

		void Draw(Graphics& gfx, FXMMATRIX accumulatedTransform) const noexcpt;
		void SetAppliedTransform(FXMMATRIX transform) noexcpt;
		int GetId() const noexcept { return id; }
		void ShowTree(Node*& pSelectedNode) const noexcpt;
		void ResetNode() noexcpt;

	private:
		void AddChild(std::unique_ptr<Node> pChild) noexcpt;

		std::string name{};
		int id = 0;
		std::vector<Mesh*> meshPtrs{};
		std::vector<std::unique_ptr<Node>> childPtrs{};
		XMFLOAT4X4 transform{};
		XMFLOAT4X4 appliedTransform{};
	};
}
