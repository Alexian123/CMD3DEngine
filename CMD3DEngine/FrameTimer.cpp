#include "FrameTimer.h"

using namespace CMD_3D_ENGINE;
using namespace std::chrono;

FrameTimer::FrameTimer()
{
	tp1 = system_clock::now();
	tp2 = system_clock::now();
}

float FrameTimer::update()
{
	tp2 = system_clock::now();
	duration<float> timeDiff = tp2 - tp1;
	tp1 = tp2;
	return timeDiff.count();
}