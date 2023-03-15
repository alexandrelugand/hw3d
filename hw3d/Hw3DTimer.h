#pragma once
#include "Forwards.h"

class Hw3DTimer
{
public:
	Hw3DTimer();

	float Mark();
	float Peek() const;

private:
	steady_clock::time_point last;
};
