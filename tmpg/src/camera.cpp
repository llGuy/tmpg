#include "camera.h"

namespace tmpg {

	void Camera::Bind(uint32_t id)
	{
		m_boundID = id;
	}

	void Camera::UpdateCursorPosition(const glm::vec2& pos)
	{
		m_cursorPosition = pos;
	}

	void Camera::Look(Entity& entity, const glm::vec2& cursor, float sensitivity)
	{
		static constexpr glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec2 cursorPosDiff = cursor - m_cursorPosition;
		m_cursorPosition = cursor;

		glm::vec3& edir = entity.Direction();
		glm::vec3 eye = entity.EyePosition();

		float xAngle = glm::radians(-cursorPosDiff.x) * sensitivity;
		edir = glm::mat3(glm::rotate(xAngle, UP)) * edir;

		glm::vec3 rotateYAx = glm::cross(edir, UP);
		float yAngle = glm::radians(-cursorPosDiff.y) * sensitivity;
		edir = glm::mat3(glm::rotate(yAngle, rotateYAx)) * edir;
	}

	uint32_t Camera::BoundEntity(void) const
	{
		return m_boundID;
	}

	glm::mat4& Camera::ViewMatrix(void) 
	{
		return m_viewMatrix;
	}

	glm::mat4& Camera::UpdateViewMatrix(Entity& ent)
	{
		static constexpr glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
		return (m_viewMatrix = glm::lookAt(ent.EyePosition(), ent.EyePosition() + ent.Direction(), UP));
	}

}