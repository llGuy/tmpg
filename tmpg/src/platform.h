#pragma once

#include <optional>
#include "mesh_3D.h"

#define PLATFORM_X 64
#define PLATFORM_Z 64

namespace tmpg {

	class Platform
	{
	public:
		Platform(void);

		void Generate(void);
		Renderable3D* Mesh(void);

		// get x and z coord of player to return the height
		float HeightAtPoint(float x, float z);
		glm::ivec2 WorldtoMeshSpace(const glm::vec2& pos);
		std::optional<glm::vec3*> At(uint32_t x, uint32_t z);

		uint32_t VIndex(uint32_t x, uint32_t z);
		uint32_t VIndexf(float x, float z);
	private:
		Renderable3D* m_mesh;
	};

}