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
		void BindCamera(uint32_t index);
		void PushEntity(const glm::vec3&, const glm::vec3&);

		Entity& operator[](uint32_t index);
		Renderable3D& Model3D(void);
		uint32_t Size(void) const;
		uint32_t EntityBoundByCamera(void) const;
		const ::gl::Program<3, 4>& Program(void) const;
	private:
		void InitShaders(void);
	private:
		::gl::Program<3, 4> m_entitiesProgram;
		std::vector<Entity> m_entities;
		EntityModel3D m_entity3DModel;
		Camera m_camera;
	};

}

#endif