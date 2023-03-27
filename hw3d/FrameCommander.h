#pragma once
#include "BlurPack.h"
#include "DepthStencil.h"
#include "RenderTarget.h"

class FrameCommander
{
public:
	FrameCommander(Graphics& gfx);

	void Accept(Job& job, size_t targetPass);
	void Execute(Graphics& gfx) noexcpt;
	void Reset() noexcept;

	void ShowWindows(Graphics& gfx);

private:
	std::array<Pass, 3> passes{};
	int downFactor = 1;
	DepthStencil ds;
	std::optional<RenderTarget> rt1;
	std::optional<RenderTarget> rt2;
	BlurPack blur;

	std::shared_ptr<Bind::VertexBuffer> pVbFull;
	std::shared_ptr<Bind::IndexBuffer> pIbFull;
	std::shared_ptr<Bind::VertexShader> pVsFull;
	std::shared_ptr<Bind::InputLayout> pLayoutFull;
	std::shared_ptr<Bind::Sampler> pSamplerFullPoint;
	std::shared_ptr<Bind::Sampler> pSamplerFullAnistropic;
	std::shared_ptr<Bind::Blender> pBlenderMerge;
};
