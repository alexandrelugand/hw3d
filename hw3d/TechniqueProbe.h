#pragma once
#include "Forwards.h"

namespace Probes
{
	class TechniqueProbe
	{
	public:
		TechniqueProbe() = default;
		virtual ~TechniqueProbe() = default;

		void SetTechnique(Rgph::Technique* pTech_in);
		void SetStep(Rgph::Step* pStep_in);

		bool VisitBuffer(Dcb::Buffer& buf);

	protected:
		virtual void OnSetTechnique();
		virtual void OnSetStep();
		virtual bool OnVisitBuffer(Dcb::Buffer& buffer);

		Rgph::Technique* pTech = nullptr;
		Rgph::Step* pStep = nullptr;
		size_t techIdx = std::numeric_limits<size_t>::max();
		size_t stepIdx = std::numeric_limits<size_t>::max();
		size_t bufIdx = std::numeric_limits<size_t>::max();
	};
}
