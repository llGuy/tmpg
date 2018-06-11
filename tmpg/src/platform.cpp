#include "platform.h"

namespace {

	float BarryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
	{
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

}

namespace tmpg {

	Platform::Platform(void)
	{
	}

	void Platform::Generate(void)
	{
		m_mesh = new Mesh3D<PLATFORM_X, PLATFORM_Z>;
		m_mesh->GenerateData();
	}

	Renderable3D* Platform::Mesh(void)
	{
		return m_mesh;
	}

	float Platform::HeightAtPoint(float x, float z)
	{
		float dmxf = static_cast<float>(PLATFORM_X) / 2.0f - 1.0f;
		float dmzf = static_cast<float>(PLATFORM_Z) / 2.0f - 1.0f;

		if (x > dmxf || x < -dmxf || z > dmzf || z < -dmzf) return -200.0f;
		float height = 0.0f;
		// get the difference between the tile corner and
		// the actual coordinate
		glm::vec2 positionOnTile(x - floor(x), z - floor(z));
		// indices of the triangle of the tile that the position
		// is pointing to
		uint16_t triangleIndices[3];

		// calculate the neg-x neg-z coord of tile
		glm::vec2 tilenegxz(floor(x) + static_cast<float>(PLATFORM_X) / 2.0f,
			floor(z) + static_cast<float>(PLATFORM_Z) / 2.0f);

		auto getheight = [&](const glm::vec2& c2, const glm::vec2& c3) -> float
		{
			triangleIndices[0] = VIndexf(tilenegxz.x, tilenegxz.y);
			triangleIndices[1] = VIndexf(tilenegxz.x + c2.x, tilenegxz.y + c2.y);
			triangleIndices[2] = VIndexf(tilenegxz.x + c3.x, tilenegxz.y + c3.y);

			glm::vec3 coord1(0.0f, m_mesh->Vertex(triangleIndices[0])->y, 0.0f);
			glm::vec3 coord2(c2.x, m_mesh->Vertex(triangleIndices[1])->y, c2.y);
			glm::vec3 coord3(c3.x, m_mesh->Vertex(triangleIndices[2])->y, c3.y);
			return BarryCentric(coord1, coord2, coord3, glm::vec2(positionOnTile.x, positionOnTile.y));
		};

		if (positionOnTile.y <= positionOnTile.x) height = getheight(glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f));
		else height = getheight(glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f));

		return height;
	}

	glm::ivec2 Platform::WorldtoMeshSpace(const glm::vec2& pos)
	{
		glm::vec2 fpos = pos + glm::vec2(static_cast<float>(PLATFORM_X) / 2.0f, static_cast<float>(PLATFORM_Z) / 2.0f);
		return glm::ivec2(static_cast<uint32_t>(fpos.x), static_cast<uint32_t>(fpos.y));
	}

	std::optional<glm::vec3*> Platform::At(uint32_t x, uint32_t z)
	{
		if (x > PLATFORM_X - 1 || z > PLATFORM_Z - 1) return std::optional<glm::vec3*>();

		uint32_t index = VIndex(x, z);
		return m_mesh->Vertex(index);
	}

	uint32_t Platform::VIndex(uint32_t x, uint32_t z)
	{
		return x + PLATFORM_X * z;
	}

	uint32_t Platform::VIndexf(float x, float z)
	{
		return static_cast<uint32_t>(x) + PLATFORM_X * static_cast<uint32_t>(z);
	}

	const ForcePoint& Platform::FP(int32_t index)
	{
		return m_forcePoints[index].fp;
	}

	void Platform::UpdateForcePoints(float timedelta)
	{
		if (m_forcePoints.size() > 0)
		{
			for (uint32_t i = 0; i < m_forcePoints.size(); ++i)
			{
				UpdateFP(i, timedelta);
			}
			m_mesh->Update();
		}
	}

	void Platform::UpdateFP(uint32_t fp, float timedelta)
	{
		ForcePoint& fpStruct = m_forcePoints[fp].fp;

		auto updateQuarter = [&](int32_t quotx, int32_t quotz, bool updateCenter, bool updateRadius) -> void
		{
			for (uint32_t moundindex = 0; moundindex < m_mound.Size(); ++moundindex)
			{
				auto mp = m_mound.At(moundindex, fpStruct.position, quotx, quotz);

				bool updateVertex = true;
				if (!updateRadius)
					updateVertex &= !(mp.terrainSpacePos.x == fpStruct.position.x || mp.terrainSpacePos.y == fpStruct.position.y);
				if (!updateCenter)
					updateVertex &= !(mp.terrainSpacePos == fpStruct.position);
				if (updateVertex)
				{
					auto vertex = At(mp.terrainSpacePos.x, mp.terrainSpacePos.y);
					if (vertex.has_value())
					{
						glm::vec3& v = *vertex.value();
						float newHeight = v.y + mp.quotient * fpStruct.intensity;
						if (v.y < newHeight)
							v.y += mp.quotient * fpStruct.intensity * timedelta;
					}
				}
			}
		};

		updateQuarter(1, 1, true, true);
		updateQuarter(1, -1, false, false);
		updateQuarter(-1, 1, false, false);
		updateQuarter(-1, -1, false, true);
	}

	void Platform::HandleAction(action_t action, Entity& entity)
	{
		using maths::Ray;

		switch (action)
		{
		case action_t::START_TERRAFORMING:
			if (entity.Terraforming() == -1)
			{
				for (Ray ray(entity.Direction(), entity.EyePosition(), 0.25f, 6.0f); ray.DistanceCovered() < ray.DistancetoEnd(); ray.Extend())
				{
					decltype(auto) currentPosition = ray.CurrentPosition();
					float terrainHeightatRay = HeightAtPoint(currentPosition.x, currentPosition.z);
					if (utils::Equf(terrainHeightatRay, currentPosition.y) || terrainHeightatRay > currentPosition.y)
					{
						float x = glm::round(currentPosition.x);
						float z = glm::round(currentPosition.z);
						glm::ivec2 meshSpace = WorldtoMeshSpace(glm::vec2(x, z));

						entity.Terraforming() = m_forcePoints.size();
						m_forcePoints.push_back({ ForcePoint(meshSpace, 8.0f), entity.ID() });
						break;
					}
				}
			}
			break;
		case action_t::END_TERRAFORMING:
			// only if the entity is currently terraforming
			if (entity.Terraforming() != -1)
			{
				m_forcePoints.erase(m_forcePoints.begin() + entity.Terraforming());
				entity.Terraforming() = -1;
			}
			break;
		}
	}

}