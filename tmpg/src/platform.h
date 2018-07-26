#pragma once

#include <vector>
#include <optional>
#include <utility>

#include "platform_shield.h"
#include "platform_mound.h"
#include "mesh_3D.h"
#include "entity.h"
#include "timer.h"
#include "utils.h"
#include "ray.h"


#define PLATFORM_X 64
#define PLATFORM_Z 64

namespace tmpg {

	enum action_t
	{
		START_TERRAFORMING,

		END_TERRAFORMING,

		SHIELD
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

	struct ProtectionPoint
	{
		ProtectionPoint(void)
			: position(-0xff, -0xff), intensity(-64.0f)
		{
		}

		ProtectionPoint(const glm::ivec2& pos, float i, float time)
			: position(pos), intensity(i), start(time)
		{
		}
		glm::ivec2 position;
		float intensity;
		float start;
	};

	struct EntityMappedFP
	{
		ForcePoint fp;
		uint32_t entityID;
	};

	struct EntityMappedPP
	{
		ProtectionPoint pp;
		uint32_t entityID;
	};

	class EntitiesHandler;

	class Platform
	{
	public:
		Platform(void);

		void Generate(void);
		Renderable3D* Mesh(void);

		bool& UpdatedExternally(void) { return m_updatedExternally; }

		// handling input
		void HandleAction(action_t action, Entity& entity);

		// get x and z coord of player to return the height
		std::pair<float, glm::vec3> HeightAtPoint(float x, float z);
		glm::ivec2 WorldtoMeshSpace(const glm::vec2& pos);
		std::optional<glm::vec3*> At(uint32_t x, uint32_t z);

		uint32_t VIndex(uint32_t x, uint32_t z);
		uint32_t VIndexf(float x, float z);
	public:
		void UpdateForcePoints(float timedelta);
		void UpdateMesh(void);
		void UpdateProtectionPoints(float timedelta, EntitiesHandler& ehandler);
		void UpdateFP(uint32_t fp, float timedelta);
		void UpdatePP(uint32_t i, float timedelta, EntitiesHandler& ehandler);
		const ForcePoint& FP(int32_t index);
	private:
		Renderable3D * m_mesh;
		// force points from which terraforming happens
		std::vector<EntityMappedFP> m_forcePoints;

		// needs a timer to check that the protection point didn't last over a certain time
		std::vector<EntityMappedPP> m_protectionPoints;
		Timer m_ppTimer;
		Mound<5> m_mound;
		Shield<5> m_shield;

		bool m_updatedExternally;
	};

}
