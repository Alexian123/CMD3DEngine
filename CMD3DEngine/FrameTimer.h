#pragma once

#include <chrono>

namespace CMD_3D_ENGINE
{
	class FrameTimer
	{
	private:
		std::chrono::system_clock::time_point tp1;
		std::chrono::system_clock::time_point tp2;
	public:
		FrameTimer();
		float update();	// returns the elapsed time
	};
}

