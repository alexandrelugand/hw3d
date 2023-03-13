#pragma once

class Hw3DTimer
{
public:
	Hw3DTimer();

	float Mark();
	float Peek() const;

private:
	std::chrono::steady_clock::time_point last;
};
