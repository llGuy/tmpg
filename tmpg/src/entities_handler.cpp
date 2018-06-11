#include "entities_handler.h"
#include "utils.h"

namespace tmpg {

	EntitiesHandler::EntitiesHandler(void)
	{
	}

	void EntitiesHandler::Init(void)
	{
		m_payerModel = new EntityModel3D(ENTITY_MODEL_RADIUS);
		m_payerModel->GenerateData();
		m_bulletTimer.Start();
		m_toggleThirdPersonTimer.Start();
	}

	void EntitiesHandler::UpdateEntities(float gravity, float time, Platform& platform)
	{
		std::for_each(m_players.begin(), m_players.end(),
			[&](Player& player) -> void 
		{ 
			decltype(auto) position = player.Position();
			player.Update(gravity, time, platform.HeightAtPoint(position.x, position.z)); 
		});

		std::for_each(m_bullets.begin(), m_bullets.end(),
			[&](Bullet& bullet) -> void { bullet.Update(gravity, time, 0.0f); 
		/* ground height collision gets calculated separately */ });

		uint32_t eliminatedBullets = 0;
		for (uint32_t i = 0; i < m_bullets.size(); ++i)
		{
			auto bcrv = QueryBulletCollision(platform, m_bullets[i]);
			if (std::get<bool>(bcrv))
			{
				m_bullets[i] = m_bullets[m_bullets.size() - eliminatedBullets - 1];
				eliminatedBullets++;
			}
		}
		for (uint32_t i = 0; i < eliminatedBullets; ++i)
		{
			m_bullets.pop_back();
		}

		m_camera.UpdateViewMatrix(PlayerBoundByCamera());
	}

	void EntitiesHandler::ToggleThirdPerson(void)
	{
		if (m_toggleThirdPersonTimer.Elapsed() > 0.1f)
		{
			m_camera.ToggleFirstThirdPerson();
			m_toggleThirdPersonTimer.Reset();
		}
	}

	EntitiesHandler::BulletCollisionRV EntitiesHandler::QueryBulletCollision(Platform& platform, Bullet& bullet)
	{
		// if bullet hit the terrain
		float platformHeight = platform.HeightAtPoint(bullet.Position().x, bullet.Position().z);
		if (utils::Equf(platformHeight, bullet.Position().y) || platformHeight > bullet.Position().y)
			return std::make_tuple<bool, std::optional<uint32_t>>(true, std::optional<uint32_t>{ /* empty */ });

		uint32_t playerIndex = 0;
		for (; playerIndex < m_players.size(); ++playerIndex)
		{
			float distance = glm::distance(bullet.Position(), m_players[playerIndex].EyePosition());
			if (distance < BULLET_MODEL_RADIUS + ENTITY_MODEL_RADIUS)
				return std::make_tuple<bool, std::optional<uint32_t>>(true, std::optional<uint32_t>(playerIndex));
		}
		return std::make_tuple<bool, std::optional<uint32_t>>(false, std::optional<uint32_t>());
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
		if (m_bulletTimer.Elapsed() > 0.1f)
		{
			Player& bound = PlayerBoundByCamera();
			m_bullets.emplace_back(bound.EyePosition(), bound.Direction());
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

	bool EntitiesHandler::ThirdPerson(void)
	{
		return m_camera.ThirdPerson();
	}

}