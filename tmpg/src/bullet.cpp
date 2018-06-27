#include "bullet.h"
#include <stdio.h>
#include <iostream>

namespace tmpg {

    Bullet::Bullet(const glm::vec3& p, const glm::vec3& d)
	: Entity(p, d, 30.0f), m_resistance(1.0f), m_rotationIntensity(200.0f), m_angle(0.0f), m_static(false)
    {
	m_position += m_direction;
    }

    void Bullet::Update(float gravity, float time, float groundHeight, glm::vec3 normal)
    {
	if(!m_static)
	{
	    // update position
	    m_position += m_direction * m_speed * time;
	    m_direction.y += gravity * time * 0.1f;
	    m_direction = glm::normalize(m_direction);
	}
    }

    float Bullet::Angle(float time) 
    {
	if(m_static) return m_angle;

	m_angle += time * m_rotationIntensity;
	if (m_angle > 360.0f) m_angle = 0.0f;
	return m_angle;
    }

    Timer& Bullet::TimeHandler(void)
    {
	return m_timer;
    }

    bool& Bullet::Static(void)
    {
	return m_static;
    }

    uint32_t& Bullet::Bounces(void)
    {
	return m_bounces;
    }

}
