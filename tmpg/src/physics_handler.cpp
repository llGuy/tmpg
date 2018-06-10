#include "physics_handler.h"

namespace tmpg { namespace physics {

	PhysicsHandler::PhysicsHandler(void)
		: m_sunPosition(30.0f, 100.0f, 30.0f)
	{
	}

	glm::vec3& PhysicsHandler::SunPosition(void) 
	{
		return m_sunPosition;
	}

}	}