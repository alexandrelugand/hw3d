#pragma once
#include "DepthStencil.h"
#include "RenderTarget.h"

class FrameCommander
{
public:
	FrameCommander(Graphics& gfx);

	void Accept(Job& job, size_t targetPass);
	void Execute(Graphics& gfx) const noexcpt;
	void Reset() noexcept;

private:
	std::array<Pass, 3> passes{};
	DepthStencil ds;
	RenderTarget rt;
	std::shared_ptr<Bind::VertexBuffer> pVbFull;
	std::shared_ptr<Bind::IndexBuffer> pIbFull;
	std::shared_ptr<Bind::VertexShader> pVsFull;
	std::shared_ptr<Bind::PixelShader> pPsFull;
	std::shared_ptr<Bind::InputLayout> pLayoutFull;
	std::shared_ptr<Bind::Sampler> pSamplerFull;
	std::shared_ptr<Bind::Blender> pBlenderFull;
};
