#pragma once
#include "Forwards.h"

class TechniqueProbe
{
public:
	TechniqueProbe() = default;
	virtual ~TechniqueProbe() = default;

	void SetTechnique(Technique* pTech_in);
	void SetStep(Step* pStep_in);

	bool VisitBuffer(Dcb::Buffer& buf);

protected:
	virtual void OnSetTechnique()
	{
	}

	virtual void OnSetStep()
	{
	}

	virtual bool OnVisitBuffer(Dcb::Buffer& buffer) = 0;

	Technique* pTech = nullptr;
	Step* pStep = nullptr;
	size_t techId = std::numeric_limits<size_t>::max();
	size_t stepId = std::numeric_limits<size_t>::max();
	size_t bufId = std::numeric_limits<size_t>::max();
};
