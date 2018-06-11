#include "player.h"

namespace tmpg {

	Player::Player(const glm::vec3& p, const glm::vec3& d, uint32_t id)
		: Entity(p, d, id, 4.0f, 0.5f)
	{
	}

	void Player::Update(float gravity, float timedelta)
	{
		
	}

	void Player::Move(movement_t m, float time)
	{
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

}