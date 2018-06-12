#ifndef	_MESH_3D_H_
#define _MESH_3D_H_

#include <array>

#include "renderable_3D.h"

namespace tmpg {

	template<uint32_t _Dmx, uint32_t _Dmz>
	class Mesh3D
		: public Renderable3D
	{
	private:
		static constexpr uint32_t VERTX = _Dmx;
		static constexpr uint32_t VERTZ = _Dmz;
	public:
		Mesh3D(void)
		{
		}
		void GenerateData(void) override
		{
			GenerateVertices();
			GenerateIndices();

			CreateVIBuffer();
			CreateVAO();
		}

		GLenum BindingPoint(void) override
		{
			return GL_ELEMENT_ARRAY_BUFFER;
		}

		uint32_t Count(void) override
		{
			static constexpr uint32_t COUNT = (VERTX - 1) * (VERTZ - 1) * 6;
			return COUNT;
		}

		uint32_t Offset(void) override
		{
			return m_vertices.size() * sizeof(glm::vec3);
		}

		glm::vec3* Vertex(uint32_t index) override
		{
			return &m_vertices[index];
		}

		void Update(void) override
		{
			UpdateVertexBuffer();
		}

		void UpdateVertexBuffer(void)
		{
			const uint32_t VERTICES_SIZE_BYTES = sizeof(glm::vec3) * m_vertices.size();
			m_buffer.PartialFill(0, VERTICES_SIZE_BYTES, m_vertices.data(), GL_ARRAY_BUFFER);
		}
	private:
		uint32_t VIndex(uint32_t x, uint32_t z)
		{
			return x + VERTX * z;
		}

		uint32_t VIndexf(float x, float z)
		{
			return static_cast<uint32_t>(x) + VERTX * static_cast<uint32_t>(z);
		}

		void GenerateVertices(void)
		{
			for (uint32_t zi = 0; zi < VERTZ; ++zi)
			{
				for (uint32_t xi = 0; xi < VERTX; ++xi)
				{
					float x = static_cast<float>(xi) - static_cast<float>(VERTX) / 2.0f;
					float z = static_cast<float>(zi) - static_cast<float>(VERTZ) / 2.0f;

					uint32_t index = VIndex(xi, zi);

					// calculate height with some generator
					m_vertices[index] = glm::vec3(x, 0.0f, z);
				}
			}
		}
		void GenerateIndices(void)
		{
			uint32_t index = 0;
			for (uint32_t gsz = 0; gsz < VERTZ - 1; ++gsz)
			{
				for (uint32_t gsx = 0; gsx < VERTX - 1; ++gsx)
				{
					m_indices[index++] = VIndex(gsx, gsz);
					m_indices[index++] = VIndex(gsx + 1, gsz);
					m_indices[index++] = VIndex(gsx + 1, gsz + 1);

					m_indices[index++] = VIndex(gsx, gsz);
					m_indices[index++] = VIndex(gsx + 1, gsz + 1);
					m_indices[index++] = VIndex(gsx, gsz + 1);
				}
			}
		}
		void GenerateColors(void)
		{
			std::fill(m_colors.begin(), m_colors.end(), glm::vec3(0.3f, 0.3f, 0.3f));
		}
	private:
		void CreateVIBuffer(void)
		{
			const uint32_t VERTICES_SIZE_BYTES = sizeof(glm::vec3) * m_vertices.size();
			const uint32_t INDICES_SIZE_BYTES = sizeof(uint16_t) * m_indices.size();

			m_buffer.Fill<void>(VERTICES_SIZE_BYTES + INDICES_SIZE_BYTES, nullptr, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER);
			m_buffer.PartialFill(0, VERTICES_SIZE_BYTES, m_vertices.data(), GL_ARRAY_BUFFER);
			m_buffer.PartialFill(VERTICES_SIZE_BYTES, INDICES_SIZE_BYTES, m_indices.data(), GL_ELEMENT_ARRAY_BUFFER);
		}
		void CreateColorBuffer(void)
		{
			m_colorBuffer.Fill(m_colors.size(), m_colors.data(), GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER);
		}
		void CreateVAO(void)
		{
			m_vao.Bind();
			m_buffer.Bind(GL_ARRAY_BUFFER);
			m_vao.Enable(m_vaoCount);
			m_vao.VAPtr(m_vaoCount, 3, GL_FLOAT, 3 * sizeof(float), nullptr);
			m_vaoCount++;
		}
	private:
		std::array<glm::vec3, VERTX * VERTZ> m_vertices;
		std::array<uint16_t, (VERTX - 1) * (VERTZ - 1) * 6> m_indices;
	};

}

#endif