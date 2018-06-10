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

}