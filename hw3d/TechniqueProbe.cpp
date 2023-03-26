#include "stdafx.h"
#include "TechniqueProbe.h"

void TechniqueProbe::SetTechnique(Technique* pTech_in)
{
	pTech = pTech_in;
	techIdx++;
	OnSetTechnique();
}

void TechniqueProbe::SetStep(Step* pStep_in)
{
	pStep = pStep_in;
	stepIdx++;
	OnSetStep();
}

bool TechniqueProbe::VisitBuffer(Dcb::Buffer& buf)
{
	bufIdx++;
	return OnVisitBuffer(buf);
}

void TechniqueProbe::OnSetTechnique()
{
}

void TechniqueProbe::OnSetStep()
{
}

bool TechniqueProbe::OnVisitBuffer(Dcb::Buffer& buffer)
{
	return false;
}
