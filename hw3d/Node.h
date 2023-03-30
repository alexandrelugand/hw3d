#pragma once

namespace Entities
{
	class Node
	{
		friend class Model;

	public:
		Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const XMMATRIX& transform_in) noexcpt;

		void Submit(FXMMATRIX accumulatedTransform) const noexcpt;

		const XMFLOAT4X4& GetAppliedTransform() const noexcept;
		void SetAppliedTransform(FXMMATRIX transform) noexcpt;

		int GetId() const noexcept;
		const std::string& GetName() const;

		bool HasChildren() const noexcept;

		void Accept(Probes::ModelProbe& probe);
		void Accept(Probes::TechniqueProbe& probe) const;

	private:
		void AddChild(std::unique_ptr<Node> pChild) noexcpt;

		int id = 0;
		std::string name{};
		std::vector<Mesh*> meshPtrs{};
		std::vector<std::unique_ptr<Node>> childPtrs{};
		XMFLOAT4X4 transform{};
		XMFLOAT4X4 appliedTransform{};
	};
}
