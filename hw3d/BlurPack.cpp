#include "stdafx.h"
#include "BlurPack.h"

BlurPack::BlurPack(Graphics& gfx, int radius, float sigma, const char* shader)
	:
	radius(radius),
	sigma(sigma),
	shader(gfx, shader),
	pcb(gfx, 0u),
	ccb(gfx, 1u)
{
	SetKernelGauss(gfx, radius, sigma);
}

void BlurPack::Bind(Graphics& gfx) noexcept
{
	shader.Bind(gfx);
	pcb.Bind(gfx);
	ccb.Bind(gfx);
}

void BlurPack::SetHorizontal(Graphics& gfx)
{
	ccb.Update(gfx, {TRUE});
}

void BlurPack::SetVertical(Graphics& gfx)
{
	ccb.Update(gfx, {FALSE});
}

void BlurPack::RenderWidgets(Graphics& gfx)
{
	bool filterChanged = false;
	{
		const char* items[] = {"Gauss", "Box"};
		static const char* curItem = items[0];
		if (ImGui::BeginCombo("Filter Type", curItem))
		{
			for (int n = 0; n < std::size(items); n++)
			{
				const bool isSelected = (curItem == items[n]);
				if (ImGui::Selectable(items[n], isSelected))
				{
					filterChanged = true;
					curItem = items[n];
					if (curItem == items[0])
					{
						kernelType = KernelType::Gauss;
					}
					else if (curItem == items[1])
					{
						kernelType = KernelType::Box;
					}
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

	bool radChange = ImGui::SliderInt("Radius", &radius, 0, 15);
	bool sigChange = ImGui::SliderFloat("Sigma", &sigma, 0.1f, 10.0f);
	if (radChange || sigChange || filterChanged)
	{
		if (kernelType == KernelType::Gauss)
		{
			SetKernelGauss(gfx, radius, sigma);
		}
		else if (kernelType == KernelType::Box)
		{
			SetKernelBox(gfx, radius);
		}
	}
}

void BlurPack::SetKernelGauss(Graphics& gfx, int radius, float sigma) noexcpt
{
	assert(radius <= maxRadius);
	Kernel k{};
	k.nTaps = radius * 2 + 1;
	float sum = 0.0f;
	for (int i = 0; i < k.nTaps; i++)
	{
		const auto x = static_cast<float>(i - radius);
		const auto g = gauss(x, sigma);
		sum += g;
		k.coefficients[i].x = g;
	}
	for (int i = 0; i < k.nTaps; i++)
	{
		k.coefficients[i].x /= sum;
	}
	pcb.Update(gfx, k);
}

void BlurPack::SetKernelBox(Graphics& gfx, int radius) noexcpt
{
	assert(radius <= maxRadius);
	Kernel k{};
	k.nTaps = radius * 2 + 1;
	const float c = 1.0f / static_cast<float>(k.nTaps);
	for (int i = 0; i < k.nTaps; i++)
	{
		k.coefficients[i].x = c;
	}
	pcb.Update(gfx, k);
}
