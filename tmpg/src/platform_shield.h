#ifndef _PLATFORM_SHIELD_H_
#define _PLATFORM_SHIELD_H_

#include <glm/glm.hpp>

namespace tmpg {

	struct ShieldPoint
	{
		glm::ivec2 position;
		float quotient;
	};

	template<uint32_t _Rad>
	class Shield
	{
	public:
		Shield(void)
		{
			for (uint32_t z = 0; z < _Rad; ++z)
			{
				for (uint32_t x = 0; x < _Rad; ++x)
				{
					float radf = static_cast<float>(_Rad);
					float xf = static_cast<float>(x);
					float zf = static_cast<float>(z);
					float distance = glm::distance(glm::vec2(xf, zf), glm::vec2(0.0f));

					// only get the circumference of the circle
					if (distance > radf - 2.0f && distance <= radf)
					{
						m_diffsFromCenter.push_back({ glm::ivec2(x, z), std::cos((distance / radf) * 1.5f) });
					}
				}
			}
		}

		uint32_t Size(void) const
		{
			return m_diffsFromCenter.size();
		}
		glm::ivec2& operator[](uint32_t index)
		{
			return m_diffsFromCenter[index];
		}

		ShieldPoint At(uint32_t index, const glm::ivec2& point, int32_t xquot, int32_t zquot)
		{
			ShieldPoint pp = m_diffsFromCenter[index];
			pp.position.x *= xquot;
			pp.position.y *= zquot;
			pp.position += point;

			return pp;
		}
	private:
		std::vector<ShieldPoint> m_diffsFromCenter;
	};

}

#endif