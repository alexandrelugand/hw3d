#pragma once
#include "Forwards.h"

class FrameCommander
{
public:
	void Accept(Job& job, size_t targetPass);
	void Execute(Graphics& gfx) const noexcpt;
	void Reset() noexcept;

private:
	std::array<Pass, 3> passes{};
};
