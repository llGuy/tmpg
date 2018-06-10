#ifndef _UTILS_H_
#define _UTILS_H_

namespace utils {

	namespace {

		const bool Equf(float a, float b)
		{
			return (fabs(a - b) < 0.00001f);
		}

	}

}

#endif