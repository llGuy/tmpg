#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <utility>
#include <glm/glm.hpp>

namespace tmpg {

	enum movement_t
	{
		FORWARD, BACKWARD, LEFT, RIGHT, JUMP, DOWN, RUN
	};

	class Entity
	{
	public:
		Entity(const glm::vec3&, const glm::vec3&, uint32_t id,
			float speed, float height);
		Entity(const glm::vec3&, const glm::vec3&, float speed);

		virtual void Update(float gravity, float time, float groundHeight);
		virtual void Move(movement_t m, float time, float gravity);

		glm::vec3 EyePosition(void) const;
		// entities can terraform
		int32_t& Terraforming(void);
		int32_t& Protected(void);
		glm::vec3& Direction(void);
		glm::vec3& Position(void);
		float& GroundHeight(void);
		float Height(void);
		uint32_t ID(void);
	protected:
		uint32_t m_id;
		glm::vec3 m_position;
		glm::vec3 m_direction;
		float m_speed;
		float m_height;
		float m_groundHeight;
		int32_t m_terraforming;
		int32_t m_protected;
	};

}

#endif