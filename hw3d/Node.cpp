#include "stdafx.h"
#include "Node.h"

namespace Entities
{
	Node::Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const XMMATRIX& transform_in) noexcpt
		: id(id), name(name), meshPtrs(meshPtrs)
	{
		XMStoreFloat4x4(&transform, transform_in);
		XMStoreFloat4x4(&appliedTransform, XMMatrixIdentity());
	}

	void Node::Submit(size_t channelFilter, FXMMATRIX accumulatedTransform) const noexcpt
	{
		const auto built = XMLoadFloat4x4(&appliedTransform) *
			XMLoadFloat4x4(&transform) *
			accumulatedTransform;

		for (const auto pm : meshPtrs)
		{
			pm->Submit(channelFilter, built);
		}

		for (const auto& pc : childPtrs)
		{
			pc->Submit(channelFilter, built);
		}
	}

	const XMFLOAT4X4& Node::GetAppliedTransform() const noexcept
	{
		return appliedTransform;
	}

	void Node::SetAppliedTransform(FXMMATRIX transform) noexcpt
	{
		XMStoreFloat4x4(&appliedTransform, transform);
	}

	int Node::GetId() const noexcept
	{
		return id;
	}

	const std::string& Node::GetName() const
	{
		return name;
	}

	bool Node::HasChildren() const noexcept
	{
		return childPtrs.size() > 0;
	}

	void Node::Accept(Probes::ModelProbe& probe)
	{
		if (probe.PushNode(*this))
		{
			for (auto& cp : childPtrs)
			{
				cp->Accept(probe);
			}
			probe.PopNode(*this);
		}
	}

	void Node::Accept(Probes::TechniqueProbe& probe) const
	{
		for (auto& mp : meshPtrs)
		{
			mp->Accept(probe);
		}
	}

	void Node::AddChild(std::unique_ptr<Node> pChild) noexcpt
	{
		assert(pChild);
		childPtrs.push_back(std::move(pChild));
	}
}
