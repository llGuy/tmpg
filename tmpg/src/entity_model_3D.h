#ifndef _ENTITY_MODEL_3D_
#define _ENTITY_MODEL_3D_

#include <glm/glm.hpp>
#include "renderable_3D.h"

namespace tmpg {

	class EntityModel3D
		: public Renderable3D
	{
	public:
		EntityModel3D(float radius);
		void GenerateData(void) override;
		// element array buffer? array buffer?
		GLenum BindingPoint(void) override;
	public:
		const ::gl::VertexArray& ModelVAO(void) const override
		{
			return m_vao;
		}
		const ::gl::Buffer& ModelBuffer(void) const override
		{
			return m_ibuffer;
		}
		uint32_t Count(void) override
		{
			return 36;
		}
		uint32_t Offset(void)
		{
			return 0;
		}
	private:
		float m_radius;
		::gl::Buffer m_ibuffer;
	};

}

#endif