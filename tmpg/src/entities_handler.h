#ifndef _EHANDLER_H_
#define _EHANDLER_H_

#include <vector>

#include "bullet.h"
#include "camera.h"
#include "player.h"
#include "program.h"
#include "entity_model_3D.h"

namespace tmpg {

	class EntitiesHandler
	{
	public:
		EntitiesHandler(void);
		void Init(void);
		void UpdateEntities(float gravity, float time);
		void BindCamera(uint32_t index);
		void PushPlayer(const glm::vec3&, const glm::vec3&);
		void PushBullet(void);
	public:
		// camera-related functions
		glm::mat4& CameraViewMatrix(void);
		Player& PlayerBoundByCamera(void);
		void UpdateCameraDirection(const glm::vec2&, float);
		void UpdateCameraCursorPosition(const glm::vec2&);
		void UpdateViewMatrix(void);
	public:
		Player& operator[](uint32_t index);
		Bullet& BulletAt(uint32_t index);
		Renderable3D* Model3D(void);
		uint32_t NumPlayers(void) const;
		uint32_t NumBullets(void) const;
	private:
		std::vector<Player> m_players;
		std::vector<Bullet> m_bullets;
		Renderable3D* m_payerModel;
		Camera m_camera;
		Timer m_bulletTimer;
	};

}

#endif