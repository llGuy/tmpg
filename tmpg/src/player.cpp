#include <iostream>
#include <cmath>

#include "player.h"

namespace tmpg {

	Player::Player(const glm::vec3& p, const glm::vec3& d, uint32_t id)
		: Entity(p, d, id, 4.0f, 0.5f), m_velocity(0.0f), m_angleUnder0(false)
	{
	}

	void Player::Update(float gravity, float timedelta, float groundHeight)
	{
		m_groundHeight = groundHeight;
		// entity is not at ground height
		if (fabs(m_groundHeight - m_position.y) < 0.01f || m_groundHeight > m_position.y)
		{
			m_position.y = m_groundHeight;
			m_velocity = 0.0f;
		}
		else
		{
			m_position.y = m_position.y + m_velocity * timedelta;
			m_velocity = m_velocity + gravity * timedelta;
		}
	}

	void Player::Move(movement_t m, float time, float gravity)
	{
		glm::vec3 finalDirection;
		switch (m)
		{
		case FORWARD: finalDirection = glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z)); break;
		case BACKWARD: finalDirection = -(glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z))); break;
		case RIGHT: finalDirection = glm::cross(m_direction, UP); break;
		case LEFT: finalDirection = -glm::cross(m_direction, UP); break;
		case JUMP: 
		{
			Jump(gravity, time);
			return;
		}
		case DOWN: finalDirection = -UP;
		}

		m_position += finalDirection * m_speed * time;
	}

	void Player::Jump(float gravity, float timedelta)
	{
		// only jump if entity is at ground height
		if (fabs(m_groundHeight - m_position.y) < 0.0001f)
		{
			m_velocity = 5.0f;

			m_position.y = m_position.y + m_velocity * timedelta;
			m_velocity = m_velocity + gravity * timedelta;
		}
	}

	glm::vec3 Player::Angle(void)
	{
		glm::vec3 angles = m_direction;
		float x = asin(angles.x);
		angles.y = x;
		return glm::vec3(0.0f, angles.y, 0.0f);
	}

}