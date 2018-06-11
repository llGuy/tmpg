#include "timer.h"

namespace tmpg {

	void Timer::Start(void)
	{
		m_currentTimeStamp = std::chrono::high_resolution_clock::now();
	}

	void Timer::Reset(void)
	{
		m_currentTimeStamp = std::chrono::high_resolution_clock::now();
	}

	float Timer::Elapsed(void)
	{
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> seconds = now - m_currentTimeStamp;
		float count = seconds.count();
		m_fps = 1.0f / count;
		return count;
	}

	float Timer::FPS(void)
	{
		return m_fps;
	}

}