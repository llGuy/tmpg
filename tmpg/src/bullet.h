#ifndef _BULLET_H_
#define _BULLET_H_

#include "entity.h"
#include "timer.h"

#define BULLET_MAX_ELAPSED_TIME 4.0f

namespace tmpg {

    class Bullet
	: public Entity
    {
    public:
	Bullet(const glm::vec3&, const glm::vec3& dir);

	void Update(float gravity, float time, float groundHeight) override;
	float Angle(float);

	bool& Static(void);
	Timer& TimeHandler(void);
	uint32_t& Bounces(void);
    private:
	float m_resistance;

	// throwing the bullets, they turn
	float m_angle;
	float m_rotationIntensity;

	bool m_static;

	uint32_t m_bounces;
	// timer to test whether or not bullet has been static for too long
	Timer m_timer;
    };

}

#endif
