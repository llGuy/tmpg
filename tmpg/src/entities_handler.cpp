#include "entities_handler.h"

namespace tmpg {

	EntitiesHandler::EntitiesHandler(void)
	{
	}

	void EntitiesHandler::Init(void)
	{
		m_entity3DModel = new EntityModel3D(0.5f);
		m_entity3DModel->GenerateData();
	}

	void EntitiesHandler::UpdateEntities(void)
	{
		std::for_each(m_entities.begin(), m_entities.end(),
			[](Entity& entity) -> void { entity.Update(); });
		m_camera.UpdateViewMatrix(EntityBoundByCamera());
	}

	void EntitiesHandler::BindCamera(uint32_t index)
	{
		m_camera.Bind(index);
	}

	void EntitiesHandler::PushEntity(const glm::vec3& p, const glm::vec3& d)
	{
		m_entities.emplace_back(p, d, m_entities.size());
	}

	Entity& EntitiesHandler::operator[](uint32_t index)
	{
		return m_entities[index];
	}

	Renderable3D* EntitiesHandler::Model3D(void) 
	{
		return m_entity3DModel;
	}

	uint32_t EntitiesHandler::Size(void) const
	{
		return m_entities.size();
	}

	Entity& EntitiesHandler::EntityBoundByCamera(void)
	{
		return m_entities[m_camera.BoundEntity()];
	}

	glm::mat4& EntitiesHandler::CameraViewMatrix(void) 
	{
		return m_camera.ViewMatrix();
	}

	void EntitiesHandler::UpdateCameraDirection(const glm::vec2& cursorPosition, float sens)
	{
		m_camera.Look(EntityBoundByCamera(), cursorPosition, sens);
	}

	void EntitiesHandler::UpdateViewMatrix(void)
	{
		m_camera.UpdateViewMatrix(EntityBoundByCamera());
	}

	void EntitiesHandler::UpdateCameraCursorPosition(const glm::vec2& p)
	{
		m_camera.UpdateCursorPosition(p);
	}

}