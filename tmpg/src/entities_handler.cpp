#include "entities_handler.h"

namespace tmpg {

	EntitiesHandler::EntitiesHandler(void)
	{
	}

	void EntitiesHandler::Init(void)
	{
		m_payerModel = new EntityModel3D(0.5f);
		m_payerModel->GenerateData();
		m_bulletTimer.Start();
	}

	void EntitiesHandler::UpdateEntities(float gravity, float time)
	{
		std::for_each(m_players.begin(), m_players.end(),
			[&](Player& player) -> void { player.Update(gravity, time); });

		std::for_each(m_bullets.begin(), m_bullets.end(),
			[&](Bullet& bullet) -> void { bullet.Update(gravity, time); });
		m_camera.UpdateViewMatrix(PlayerBoundByCamera());
	}

	void EntitiesHandler::BindCamera(uint32_t index)
	{
		m_camera.Bind(index);
	}

	void EntitiesHandler::PushPlayer(const glm::vec3& p, const glm::vec3& d)
	{
		m_players.emplace_back(p, d, m_players.size());
	}

	void EntitiesHandler::PushBullet(void)
	{
		if (m_bulletTimer.Elapsed() > 0.5f)
		{
			Player& bund = PlayerBoundByCamera();
			m_bullets.emplace_back(player.Position(), player.Direction());
			m_bulletTimer.Reset();
		}
	}

	Player& EntitiesHandler::operator[](uint32_t index)
	{
		return m_players[index];
	}

	Bullet& EntitiesHandler::BulletAt(uint32_t index)
	{
		return m_bullets[index];
	}

	Renderable3D* EntitiesHandler::Model3D(void) 
	{
		return m_payerModel;
	}

	uint32_t EntitiesHandler::NumPlayers(void) const
	{
		return m_players.size();
	}
	
	uint32_t EntitiesHandler::NumBullets(void) const
	{
		return m_bullets.size();
	}

	Player& EntitiesHandler::PlayerBoundByCamera(void)
	{
		return m_players[m_camera.BoundEntity()];
	}

	glm::mat4& EntitiesHandler::CameraViewMatrix(void) 
	{
		return m_camera.ViewMatrix();
	}

	void EntitiesHandler::UpdateCameraDirection(const glm::vec2& cursorPosition, float sens)
	{
		m_camera.Look(PlayerBoundByCamera(), cursorPosition, sens);
	}

	void EntitiesHandler::UpdateViewMatrix(void)
	{
		m_camera.UpdateViewMatrix(PlayerBoundByCamera());
	}

	void EntitiesHandler::UpdateCameraCursorPosition(const glm::vec2& p)
	{
		m_camera.UpdateCursorPosition(p);
	}

}