#include "entity.h"

namespace tmpg {

	Entity::Entity(const glm::vec3& p, const glm::vec3& d, uint32_t id)
		: m_position(p), m_direction(d), m_id(id), m_speed(0.2f), m_height(0.5f)
	{
	}

	void Entity::Update(void)
	{
		// update some data
	}

	void Entity::Move(movement_t m, float time)
	{
		glm::vec3 finalDirection;
		switch (m)
		{
		case FORWARD: finalDirection = glm::normalize(glm::vec3(m_position.x, 0.0f, m_position.z)); break;
		case BACKWARD: finalDirection = -(glm::normalize(glm::vec3(m_position.x, 0.0f, m_position.z))); break;
		case RIGHT: finalDirection = glm::cross(m_position, UP); break;
		case LEFT: finalDirection = -glm::cross(m_position, UP); break;
		case JUMP: finalDirection = UP;
		}

		m_position += finalDirection * m_speed * time;
	}

	glm::vec3 Entity::EyePosition(void) const
	{
		return m_position + glm::vec3(0.0f, m_height, 0.0f);
	}

	glm::vec3& Entity::Position(void)
	{
		return m_position;
	}

	glm::vec3& Entity::Direction(void)
	{
		return m_direction;
	}

}