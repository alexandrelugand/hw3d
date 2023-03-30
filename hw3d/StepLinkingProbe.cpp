#include "stdafx.h"
#include "StepLinkingProbe.h"

namespace Probes
{
	void StepLinkingProbe::OnSetStep()
	{
		pStep->Link(rg);
	}
}
