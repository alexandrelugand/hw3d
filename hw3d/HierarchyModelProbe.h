#pragma once
#include "TransformParameters.h"

namespace Probes
{
	class HierarchyModelProbe : ModelProbe
	{
	public:
		void SpawnWindow(const Entities::Model& model);

	protected:
		bool PushNode(Entities::Node& node) override;
		void PopNode(Entities::Node& node) override;

	private:
		Entities::Node* pSelectedNode = nullptr;
		std::unordered_map<int, TransformParameters> transformParams;

		TransformParameters& ResolveTransform() noexcept;
		TransformParameters& LoadTransform(int id) noexcept;
	};
}
