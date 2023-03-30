#pragma once

namespace Probes
{
	class MeshTechniqueProbe : public TechniqueProbe
	{
	public:
		void OnSetTechnique() override;
		bool OnVisitBuffer(Dcb::Buffer& buf) override;
	};
}
