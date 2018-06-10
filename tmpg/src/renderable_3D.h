#ifndef _RENDERABLE_3D_H_
#define _RENDERABLE_3D_H_

#include "vao.h"
#include "buffer.h"

namespace tmpg {

	// describes all types of renderable
	// models treated as 3D objects
	class Renderable3D
	{
	public:
		Renderable3D(void) = default;
		virtual ~Renderable3D(void) 
		{
		}

		virtual void GenerateData(void) = 0;
		// element array buffer? array buffer?
		virtual GLenum BindingPoint(void) = 0;
		virtual uint32_t Count(void) = 0;
		virtual uint32_t Offset(void) = 0;

		// not all classes need this
		virtual void Update(void) 
		{
		}
	public:
		virtual const ::gl::VertexArray& ModelVAO(void) const
		{
			return m_vao;
		}
		virtual const ::gl::Buffer& ModelBuffer(void) const
		{
			return m_buffer;
		}
	protected:
		::gl::VertexArray m_vao;
		::gl::Buffer m_buffer;
	};

}

#endif