#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <utility>
#include "entity.h"
#include <glm/glm.hpp>

#define PLAYER_SPEED 4.0f
#define MAX_MOMENTUM 10.0f

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
		void Update(float gravity, float time, float groundHeight, glm::vec3 normal = glm::vec3(0.0f)) override;
		void Move(movement_t m, float time, float gravity) override;
		void NeutralizeMomentum(void);

		glm::vec3 Angle(void);
		std::string& Username(void);
	private:
		void Jump(float, float);
		bool AtGroundHeight(void);
	private:
		static constexpr glm::vec3 UP{ 0.0f, 1.0f, 0.0f };

		// for jumping
		float m_velocity;

		bool m_angleUnder0;
		bool m_falling;
		bool m_flying;
		float m_fallingMomentum;
		glm::vec3 m_directionMomentum;

		std::string m_name;
	};

}

#endif
