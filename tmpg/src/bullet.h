#ifndef _BULLET_H_
#define _BULLET_H_

#include "entity.h"

namespace tmpg {

	class Bullet
		: public Entity
	{
	public:
		Bullet(const glm::vec3&, const glm::vec3& dir);

		void Update(float gravity, float time) override;
	private:
		float m_resistance;
	};

}

#endif