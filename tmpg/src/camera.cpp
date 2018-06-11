#include "camera.h"

namespace tmpg {

	Camera::Camera(void)
		: m_angles(60.0f, 60.0f), m_distance(5.0f)
	{
	}

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

		if (m_thirdPerson)
		{
			m_angles.x -= cursorPosDiff.x * sensitivity;
			m_angles.y -= cursorPosDiff.y * sensitivity;

			m_direction = glm::vec3(-sin(glm::radians(m_angles.x)), -cos(glm::radians(m_angles.y)), -cos(glm::radians(m_angles.x)));

			entity.Direction() = glm::normalize(glm::vec3(m_direction.x, 0.0f, m_direction.z));
		}
		else
		{
			float xAngle = glm::radians(-cursorPosDiff.x) * sensitivity;
			float yAngle = glm::radians(-cursorPosDiff.y) * sensitivity;

			glm::vec3& edir = entity.Direction();
			glm::vec3 eye = entity.EyePosition();

			edir = glm::mat3(glm::rotate(xAngle, UP)) * edir;

			glm::vec3 rotateYAx = glm::cross(edir, UP);
			edir = glm::mat3(glm::rotate(yAngle, rotateYAx)) * edir;
		}
	}

	void Camera::ToggleFirstThirdPerson(void)
	{
		m_thirdPerson ^= true;
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

		if (m_thirdPerson)
		{
			m_position = ent.EyePosition() - m_direction * m_distance;
			glm::vec3 position = m_position;
			if (m_position.y < ent.GroundHeight() + 0.3f) position.y = ent.GroundHeight();
			return (m_viewMatrix = glm::lookAt(position, position + m_direction, UP));
		}
		else return (m_viewMatrix = glm::lookAt(ent.EyePosition(), ent.EyePosition() + ent.Direction(), UP));
	}

	bool Camera::ThirdPerson(void)
	{
		return m_thirdPerson;
	}

}