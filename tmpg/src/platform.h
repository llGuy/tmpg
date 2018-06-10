#pragma once

#include <vector>
#include <optional>

#include "platform_mound.h"
#include "mesh_3D.h"
#include "entity.h"
#include "utils.h"
#include "ray.h"

#define PLATFORM_X 64
#define PLATFORM_Z 64

namespace tmpg {

	enum action_t
	{
		START_TERRAFORMING,

		END_TERRAFORMING
	};

	struct ForcePoint
	{
		ForcePoint(void)
			: position(-0xff, -0xff), intensity(-64.0f)
		{
		}

		ForcePoint(const glm::ivec2& pos, float i)
			: position(pos), intensity(i)
		{
		}

		glm::ivec2 position;
		float intensity;
	};

	struct EntityMappedFP
	{
		ForcePoint fp;
		uint32_t entityID;
	};

	class Platform
	{
	public:
		Platform(void);

		void Generate(void);
		Renderable3D* Mesh(void);

		// handling input
		void HandleAction(action_t action, Entity& entity);

		// get x and z coord of player to return the height
		float HeightAtPoint(float x, float z);
		glm::ivec2 WorldtoMeshSpace(const glm::vec2& pos);
		std::optional<glm::vec3*> At(uint32_t x, uint32_t z);

		uint32_t VIndex(uint32_t x, uint32_t z);
		uint32_t VIndexf(float x, float z);
	public:
		void UpdateForcePoints(float timedelta);
		void UpdateFP(uint32_t fp, float timedelta);
		const ForcePoint& FP(int32_t index);
	private:
		Renderable3D* m_mesh;
		// force points from which terraforming happens
		std::vector<EntityMappedFP> m_forcePoints;
		Mound<5> m_mound;
	};

}