#ifndef	_PLATFORM_MOUND_H_
#define _PLATFORM_MOUND_H_

#include "utils.h"
#include <glm/glm.hpp>

namespace tmpg {

	struct MoundPoint
	{
		union
		{
			glm::ivec2 diffFromCenter;
			glm::ivec2 terrainSpacePos;
		};
		float quotient;
	};

	// mound of a certain radius
	template<uint32_t _Rad>
	class Mound
	{
	public:
		Mound(void)
		{
			for (uint32_t z = 0; z < _Rad; ++z)
			{
				for (uint32_t x = 0; x < _Rad; ++x)
				{
					float radf = static_cast<float>(_Rad);
					float xf = static_cast<float>(x);
					float zf = static_cast<float>(z);
					float distance = glm::distance(glm::vec2(xf, zf), glm::vec2(0.0f));
					if (distance <= radf || utils::Equf(distance, radf))
					{
						m_diffs.push_back(MoundPoint{ glm::ivec2(x, z), std::cos((distance / radf) * 1.5f) });
					}
				}
			}
		}
		uint32_t Size(void) const
		{
			return m_diffs.size();
		}
		MoundPoint& operator[](uint32_t index)
		{
			return m_diffs[index];
		}
		MoundPoint At(uint32_t index, const glm::ivec2& point, int32_t xquot, int32_t zquot)
		{
			MoundPoint mp = m_diffs[index];
			mp.terrainSpacePos.x *= xquot;
			mp.terrainSpacePos.y *= zquot;
			mp.terrainSpacePos += point;

			return mp;
		}
	private:
		// difference from the center
		std::vector<MoundPoint> m_diffs;
	};

}

#endif