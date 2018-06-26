#ifndef _BULLET_H_
#define _BULLET_H_

#include "entity.h"

namespace tmpg {

    class Bullet
	: public Entity
    {
    public:
	Bullet(const glm::vec3&, const glm::vec3& dir);

	void Update(float gravity, float time, float groundHeight) override;
	float Angle(float);

	bool& Static(void);
    private:
	float m_resistance;

	// throwing the bullets, they turn
	float m_angle;
	float m_rotationIntensity;

	bool m_static;
    };

}

#endif
