#include "bullet.h"

namespace tmpg {

	Bullet::Bullet(const glm::vec3& p, const glm::vec3& d)
		: Entity(p, d, 20.0f), m_resistance(3.0f)
	{
	}

	void Bullet::Update(float gravity, float time)
	{
		// update position
		m_position += m_direction * m_speed * time;
		m_direction.y += gravity * time * 0.1f;
		m_speed -= m_resistance * time;
	}

}