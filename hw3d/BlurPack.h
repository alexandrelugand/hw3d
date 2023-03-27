#pragma once
#include "ConstantBuffer.h"
#include "PixelShader.h"

class BlurPack
{
public:
	BlurPack(Graphics& gfx, int radius = 7, float sigma = 2.6f, const char* shader = "Blur_PS.cso");

	void Bind(Graphics& gfx) noexcept;
	void SetHorizontal(Graphics& gfx);
	void SetVertical(Graphics& gfx);

	void RenderWidgets(Graphics& gfx);

	// for more accurate coefs, need to integrate, but meh :/
	void SetKernelGauss(Graphics& gfx, int radius, float sigma) noexcpt;
	void SetKernelBox(Graphics& gfx, int radius) noexcpt;

private:
	enum class KernelType
	{
		Gauss,
		Box,
	};

	static constexpr int maxRadius = 15;
	int radius;
	float sigma;
	KernelType kernelType = KernelType::Gauss;

	struct Kernel
	{
		int nTaps;
		float padding[3];
		XMFLOAT4 coefficients[maxRadius * 2 + 1];
	};

	struct Control
	{
		BOOL horizontal;
		float padding[3];
	};

	Bind::PixelShader shader;
	Bind::PixelConstantBuffer<Kernel> pcb;
	Bind::PixelConstantBuffer<Control> ccb;
};
