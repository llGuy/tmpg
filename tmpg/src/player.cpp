#include <iostream>
#include <cmath>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "player.h"

namespace tmpg {

	Player::Player(const glm::vec3& p, const glm::vec3& d, uint32_t id)
		: Entity(p, d, id, 4.0f, 0.5f), m_velocity(0.0f), m_angleUnder0(false), m_falling(false), m_fallingMomentum(0.0f), m_flying(false),
		m_directionMomentum(0.0f)
	{
	}

	bool Player::AtGroundHeight(void)
	{
		return fabs(m_groundHeight - m_position.y) < 0.01f || m_groundHeight > m_position.y;
	}

	void Player::Update(float gravity, float timedelta, float groundHeight, glm::vec3 normal)
	{
		//printf("%s\n", glm::to_string(m_directionMomentum).c_str());

		m_groundHeight = groundHeight;
		// entity is not at ground height
		if (fabs(m_groundHeight - m_position.y) < 0.00001f || m_groundHeight > m_position.y)
		{
			m_position.y = m_groundHeight;
			m_velocity = 0.0f;
			m_flying = false;
		}
		else
		{
			m_position.y = m_position.y + m_velocity * timedelta;
			m_velocity = m_velocity + gravity * timedelta;
		}
		if (m_falling && !m_flying)
		{
			// calculate angle of inclination of terrain
			float inclineAngle = glm::angle(glm::vec3(0.0f, 1.0f, 0.0f), normal);
			glm::vec2 xzNormal = glm::vec2(normal.x, normal.z);
			glm::vec2 xzDirection = glm::normalize(glm::vec2(m_direction.x, m_direction.z));
			float directionAngle = glm::angle(xzNormal, xzDirection);
			if (fabs(m_fallingMomentum - MAX_MOMENTUM) > 0.001f)
			{
				float momentum = (inclineAngle) / glm::radians(90.0f) - 0.3f;
				momentum *= 1.0f - (directionAngle / glm::radians(90.0f));
				m_fallingMomentum += momentum * 0.5f;
				if (m_fallingMomentum > MAX_MOMENTUM) m_fallingMomentum = MAX_MOMENTUM;

				m_directionMomentum += glm::vec3(momentum * xzDirection.x, 0.0f, momentum * xzDirection.y) * 0.4f;
			}

			// if the momentum is very high and the player is going up
			if (m_fallingMomentum > 9.0f && 1.0f - (directionAngle / glm::radians(90.0f)) < 0.0f)
			{
				m_flying = true;
				m_velocity = 15.0f;
				m_directionMomentum = glm::vec3(m_directionMomentum.x, 0.0f, m_directionMomentum.z);
				m_position += m_directionMomentum * timedelta; m_position.y += m_velocity * timedelta;
			}
			else m_position.y = m_groundHeight;
		}
		if(fabs(m_velocity) > 0.000001f || m_falling)
			m_position += (m_directionMomentum) * timedelta * (!m_falling ? m_speed : 1.0f);
	}

	void Player::Move(movement_t m, float time, float gravity)
	{
		if (m == FALL)
		{
			m_falling = true; return;
		}
		else
		{
			m_fallingMomentum = 0.0f;
			m_falling = false;
		}
		glm::vec3 finalDirection(0.0f);
		switch (m)
		{
		case FORWARD: 
		{
			if (AtGroundHeight())
			{
				finalDirection = glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z));
				m_directionMomentum = finalDirection;
				break;
			}
			else return;
		}
		case BACKWARD: 
		{
			if (AtGroundHeight())
			{
				finalDirection = -(glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z))); 
				m_directionMomentum = finalDirection;
				break;
			}
			else return;
		}
		case RIGHT: 
		{
			if (AtGroundHeight())
			{
				finalDirection = glm::cross(m_direction, UP); 
				m_directionMomentum = finalDirection;
			}
			else
			{
				// rotate direction
				glm::mat3 rotation = glm::mat3(glm::rotate(glm::radians(-90.0f * time), glm::vec3(0.0f, m_speed, 0.0f)));
				m_directionMomentum = rotation * m_directionMomentum;
				m_direction = rotation * m_direction;
			}
			break;
		}
		case LEFT: 
		{
			if (AtGroundHeight())
			{
				finalDirection = -glm::cross(m_direction, UP); 
				m_directionMomentum = finalDirection;
			}
			else
			{
				// rotate direction
				glm::mat3 rotation = glm::mat3(glm::rotate(glm::radians(90.0f * time), glm::vec3(0.0f, m_speed, 0.0f)));
				m_directionMomentum = rotation * m_directionMomentum;
				m_direction = rotation * m_direction;
			}
			break;
		}
		case JUMP:
		{
			Jump(gravity, time);
			//if(glm::all(glm::greaterThan(glm::abs(m_directionMomentum - glm::vec3(0.0f)), glm::vec3(0.00001f))))
			//	m_directionMomentum = glm::normalize(m_directionMomentum);
			return;
		}
		}
		m_position += finalDirection * m_speed * time;
	}

	void Player::NeutralizeMomentum(void)
	{
		if (AtGroundHeight() && !m_falling)
		{
			m_directionMomentum = glm::vec3(0.0f);
		}
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
		float x = -atan(angles.z / angles.x);
		angles.y = x;
		return glm::vec3(0.0f, angles.y, 0.0f);
	}

	std::string& Player::Username(void)
	{
		return m_name;
	}

}
