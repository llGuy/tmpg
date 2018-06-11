#ifndef _PHYSICS_HANDLER_H_
#define _PHYSICS_HANDLER_H_

#include <glm/glm.hpp>

namespace tmpg { namespace physics {

	class PhysicsHandler
	{
	public:
		PhysicsHandler(void);

		glm::vec3& SunPosition(void);
		float Gravity(void) const;
	private:
		glm::vec3 m_sunPosition;
		float m_gravity;
	};

}	}

#endif