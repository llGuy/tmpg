#ifndef _PHYSICS_HANDLER_H_
#define _PHYSICS_HANDLER_H_

#include <glm/glm.hpp>

namespace tmpg { namespace physics {

	class PhysicsHandler
	{
	public:
		PhysicsHandler(void);

		glm::vec3& SunPosition(void);
	private:
		glm::vec3 m_sunPosition;
	};

}	}

#endif