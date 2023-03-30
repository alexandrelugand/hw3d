#pragma once

namespace Probes
{
	class ModelProbe
	{
	public:
		ModelProbe() = default;
		virtual ~ModelProbe() = default;

		virtual bool PushNode(Entities::Node& node) = 0;
		virtual void PopNode(Entities::Node& node) = 0;
	};
}
