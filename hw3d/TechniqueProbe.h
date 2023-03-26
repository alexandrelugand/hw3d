#pragma once
#include "Forwards.h"

class TechniqueProbe
{
public:
	// TODO: add callback for visiting each mesh

	TechniqueProbe() = default;
	virtual ~TechniqueProbe() = default;

	void SetTechnique(Technique* pTech_in);
	void SetStep(Step* pStep_in);

	bool VisitBuffer(Dcb::Buffer& buf);

protected:
	virtual void OnSetTechnique();
	virtual void OnSetStep();
	virtual bool OnVisitBuffer(Dcb::Buffer& buffer);

	Technique* pTech = nullptr;
	Step* pStep = nullptr;
	size_t techIdx = std::numeric_limits<size_t>::max();
	size_t stepIdx = std::numeric_limits<size_t>::max();
	size_t bufIdx = std::numeric_limits<size_t>::max();
};
