#include "entity_model_3D.h"

namespace tmpg {

	EntityModel3D::EntityModel3D(float radius)
		: m_radius(radius)
	{
	}

	void EntityModel3D::GenerateData(void)
	{
		glm::vec3 verts[]
		{
			// front
			glm::vec3(-m_radius, -m_radius,  m_radius),
			glm::vec3(m_radius, -m_radius,  m_radius),
			glm::vec3(m_radius,  m_radius,  m_radius),
			glm::vec3(-m_radius,  m_radius,  m_radius),
			// back
			glm::vec3(-m_radius, -m_radius, -m_radius),
			glm::vec3(m_radius, -m_radius, -m_radius),
			glm::vec3(m_radius,  m_radius, -m_radius),
			glm::vec3(-m_radius,  m_radius, -m_radius)
		};
		m_buffer.Fill(sizeof(verts), verts, GL_STATIC_DRAW, GL_ARRAY_BUFFER);
		m_radius = glm::distance(glm::vec3(0.0f), glm::vec3(m_radius));

		uint16_t indices[]
		{
			// front
			0, 1, 2,
			2, 3, 0,
			// right
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// left
			4, 0, 3,
			3, 7, 4,
			// bottom
			4, 5, 1,
			1, 0, 4,
			// top
			3, 2, 6,
			6, 7, 3,
		};
		m_ibuffer.Fill(sizeof(indices), indices, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);

		m_buffer.Bind(GL_ARRAY_BUFFER);

		m_vao.Bind();
		m_vao.Enable(m_vaoCount);
		m_vao.VAPtr(m_vaoCount, 3, GL_FLOAT, 3 * sizeof(float), nullptr);
		m_ibuffer.Bind(GL_ELEMENT_ARRAY_BUFFER);
		m_vao.Unbind();

		m_vaoCount++;
	}

	GLenum EntityModel3D::BindingPoint(void)
	{
		// renderer needs to bind to this point
		return GL_ELEMENT_ARRAY_BUFFER;
	}

}