#include "physics_handler.h"

namespace tmpg { namespace physics {

	PhysicsHandler::PhysicsHandler(void)
		: m_sunPosition(30.0f, 50.0f, 30.0f),
		m_gravity(-10.5f)
	{
	}

	glm::vec3& PhysicsHandler::SunPosition(void) 
	{
		return m_sunPosition;
	}

	float PhysicsHandler::Gravity(void) const
	{
		return m_gravity;
	}

}	}