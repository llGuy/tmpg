#include "entity.h"

namespace tmpg {

	Entity::Entity(const glm::vec3& p, const glm::vec3& d, uint32_t id,
		float speed, float height)
		: m_position(p), m_direction(d), m_id(id), m_speed(speed), m_height(height),
		m_terraforming(-1)
	{
	}

	Entity::Entity(const glm::vec3& p, const glm::vec3& d, float speed)
		: m_position(p), m_direction(d), m_speed(speed)
	{
	}

	void Entity::Update(float gravity, float time, float groundHeight)
	{
		
	}

	void Entity::Move(movement_t m, float time, float gravity)
	{
		static constexpr glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec3 finalDirection;
		switch (m)
		{
		case FORWARD: finalDirection = glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z)); break;
		case BACKWARD: finalDirection = -(glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z))); break;
		case RIGHT: finalDirection = glm::cross(m_direction, UP); break;
		case LEFT: finalDirection = -glm::cross(m_direction, UP); break;
		case JUMP: finalDirection = UP; break;
		case DOWN: finalDirection = -UP;
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

	uint32_t Entity::ID(void)
	{
		return m_id;
	}

	int32_t& Entity::Terraforming(void)
	{
		return m_terraforming;
	}

	float Entity::Height(void)
	{
		return m_height;
	}

	float& Entity::GroundHeight(void)
	{
		return m_groundHeight;
	}

}