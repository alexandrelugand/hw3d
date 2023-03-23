#pragma once

class Pass
{
public:
	void Accept(Job& job) noexcept;
	void Execute(Graphics& gfx) const noexcpt;
	void Reset() noexcept;

private:
	std::vector<Job> jobs{};
};
