#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "entity.h"

namespace tmpg {

	class Camera
	{
	public:
		Camera(void) = default;
		
		void Bind(uint32_t id);
		void UpdateCursorPosition(const glm::vec2& pos);
		void Look(Entity& entity, const glm::vec2& cursor, float sensitivity);
	public:
		glm::mat4& ViewMatrix(void);
		uint32_t BoundEntity(void) const;
		glm::mat4& UpdateViewMatrix(Entity& ent);
	private:
		// id of the entity that the camera is bound to
		uint32_t m_boundID;
		glm::vec2 m_cursorPosition;
		glm::mat4 m_viewMatrix;
	};

}

#endif