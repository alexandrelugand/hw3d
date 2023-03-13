#include "stdafx.h"
#include "Hw3DTimer.h"

Hw3DTimer::Hw3DTimer()
{
	last = steady_clock::now();
}

float Hw3DTimer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float Hw3DTimer::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}
