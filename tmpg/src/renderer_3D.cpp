#include "renderer_3D.h"

namespace tmpg {

	void Renderer3D::Bind(Renderable3D& obj)
	{
		m_bound = &obj;
	}

	void Renderer3D::RenderElements(GLenum mode)
	{
		const ::gl::VertexArray& vao = m_bound->ModelVAO();
		const ::gl::Buffer& buffer = m_bound->ModelBuffer();

		vao.Bind();
		buffer.Bind(m_bound->BindingPoint());

		glDrawElements(mode, m_bound->Count(), GL_UNSIGNED_SHORT, (void*)m_bound->Offset());

		buffer.Unbind(m_bound->BindingPoint());
		vao.Unbind();
	}

}