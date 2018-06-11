#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <utility>
#include "entity.h"
#include <glm/glm.hpp>

namespace tmpg {

	enum state_t
	{
		GRAVITY
	};

	// there will only be one type of entity
	class Player
		: public Entity
	{
	public:
		Player(const glm::vec3& p, const glm::vec3& d, uint32_t id);

		// update without gravity
		void Update(float gravity, float time, float groundHeight) override;
		void Move(movement_t m, float time, float gravity) override;

		glm::vec3 Angle(void);
	private:
		void Jump(float, float);
	private:
		static constexpr glm::vec3 UP { 0.0f, 1.0f, 0.0f };

		// for jumping
		float m_velocity;

		bool m_angleUnder0;
	};

}

#endif