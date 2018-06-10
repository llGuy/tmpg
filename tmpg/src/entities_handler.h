#ifndef _EHANDLER_H_
#define _EHANDLER_H_

#include <vector>

#include "camera.h"
#include "entity.h"
#include "program.h"
#include "entity_model_3D.h"

namespace tmpg {

	class EntitiesHandler
	{
	public:
		EntitiesHandler(void);
		void Init(void);
		void UpdateEntities(void);
		void BindCamera(uint32_t index);
		void PushEntity(const glm::vec3&, const glm::vec3&);
	public:
		// camera-related functions
		glm::mat4& CameraViewMatrix(void);
		Entity& EntityBoundByCamera(void);
		void UpdateCameraDirection(const glm::vec2&, float);
		void UpdateCameraCursorPosition(const glm::vec2&);
		void UpdateViewMatrix(void);
	public:
		Entity& operator[](uint32_t index);
		Renderable3D* Model3D(void);
		uint32_t Size(void) const;
	private:
		std::vector<Entity> m_entities;
		Renderable3D* m_entity3DModel;
		Camera m_camera;
	};

}

#endif