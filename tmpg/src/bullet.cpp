#include "bullet.h"

namespace tmpg {

	Bullet::Bullet(const glm::vec3& p, const glm::vec3& d)
		: Entity(p, d, 20.0f), m_resistance(3.0f), m_rotationIntensity(200.0f), m_angle(0.0f)
	{
		m_position += m_direction;
	}

	void Bullet::Update(float gravity, float time, float groundHeight)
	{
		// update position
		m_position += m_direction * m_speed * time;
		m_direction.y += gravity * time * 0.1f;
		m_speed -= m_resistance * time;
	}

	float Bullet::Angle(float time) 
	{
		m_angle += time * m_rotationIntensity;
		if (m_angle > 360.0f) m_angle = 0.0f;
		return m_angle;
	}

}