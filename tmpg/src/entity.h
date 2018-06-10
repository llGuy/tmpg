#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <glm/glm.hpp>

namespace tmpg {

	enum movement_t
	{
		FORWARD, BACKWARD, LEFT, RIGHT, JUMP, RUN
	};

	enum state_t
	{
		GRAVITY
	};

	// there will only be one type of entity
	class Entity
	{
	public:
		Entity(const glm::vec3& p, const glm::vec3& d, uint32_t id);

		// update without gravity
		void Update(void);
	public:
		void Move(movement_t m, float time);

		glm::vec3 EyePosition(void) const;
		glm::vec3& Position(void);
		glm::vec3& Direction(void);
		uint32_t ID(void);
	private:
		static constexpr glm::vec3 UP { 0.0f, 1.0f, 0.0f };

		glm::vec3 m_position;
		glm::vec3 m_direction;
		float m_speed;
		float m_height;
		uint32_t m_id;
	};

}

#endif