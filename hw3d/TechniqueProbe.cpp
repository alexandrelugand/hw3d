#include "stdafx.h"
#include "TechniqueProbe.h"

void TechniqueProbe::SetTechnique(Technique* pTech_in)
{
	pTech = pTech_in;
	techId++;
	OnSetTechnique();
}

void TechniqueProbe::SetStep(Step* pStep_in)
{
	pStep = pStep_in;
	stepId++;
	OnSetStep();
}

bool TechniqueProbe::VisitBuffer(Dcb::Buffer& buf)
{
	bufId++;
	return OnVisitBuffer(buf);
}
