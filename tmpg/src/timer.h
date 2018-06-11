#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>

namespace tmpg {

	class Timer
	{
	public:
		Timer(void) = default;
		void Start(void);
		void Reset(void);
		float Elapsed(void);
		float FPS(void);
	private:
		std::chrono::high_resolution_clock::time_point m_currentTimeStamp;
		float m_fps;
	};

}

#endif