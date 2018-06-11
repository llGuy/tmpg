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
		void Update(float gravity, float time) override;
		void Move(movement_t m, float time) override;
	private:
		void Jump(float, float);
		void GravityForce(float, float);
	private:
		static constexpr glm::vec3 UP { 0.0f, 1.0f, 0.0f };
	};

}

#endif