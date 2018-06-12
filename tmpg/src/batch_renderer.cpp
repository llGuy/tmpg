#include "batch_renderer.h"

namespace tmpg {

	BatchRenderer3D::BatchRenderer3D(uint32_t bufferSize)
		: m_bufferMaxSize(bufferSize), m_size(0)
	{
	}

	void BatchRenderer3D::Bind(Renderable3D* renderable)
	{
		m_renderable = renderable;
	}

	void BatchRenderer3D::PrepareMatrixAttribute(void)
	{
		::gl::VertexArray& vao = m_renderable->ModelVAO();
		uint32_t& vaoCount = m_renderable->VAOCount();
		vao.Bind();
		m_buffer->Bind(GL_ARRAY_BUFFER);
		uint32_t original = vaoCount;

		for (; vaoCount < original + 4; ++vaoCount)
		{
			vao.Enable(vaoCount);
			vao.Divisor(vaoCount, 1);
			vao.VAPtr(vaoCount, 4, GL_FLOAT, 4 * 4 * sizeof(float), (void*)((vaoCount - original) * sizeof(float) * 4));
		}

		vao.Unbind();
	}

	void BatchRenderer3D::EmptyBuffer(void)
	{
		m_size = 0;
		m_buffer->Fill<void>(m_bufferMaxSize, nullptr, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER);
	}

	void BatchRenderer3D::RenderElementsInstanced(GLenum mode)
	{
		::gl::VertexArray& vao = m_renderable->ModelVAO();
		const ::gl::Buffer& buffer = m_renderable->ModelBuffer();

		vao.Bind();
		buffer.Bind(m_renderable->BindingPoint());

		glDrawElementsInstanced(mode, m_renderable->Count(), GL_UNSIGNED_SHORT, (void*)m_renderable->Offset(), m_size);

		buffer.Unbind(m_renderable->BindingPoint());
		vao.Unbind();
	}

	void BatchRenderer3D::RenderElements(GLenum mode)
	{
		::gl::VertexArray& vao = m_renderable->ModelVAO();
		const ::gl::Buffer& buffer = m_renderable->ModelBuffer();

		vao.Bind();
		buffer.Bind(m_renderable->BindingPoint());

		glDrawElements(mode, m_renderable->Count(), GL_UNSIGNED_SHORT, (void*)m_renderable->Offset());

		buffer.Unbind(m_renderable->BindingPoint());
		vao.Unbind();
	}

	void BatchRenderer3D::PushMatrix(float* data)
	{
		static constexpr uint32_t MATRIX_SIZE = 4 * 4 * sizeof(float);
		m_buffer->PartialFill((m_size++) * MATRIX_SIZE, MATRIX_SIZE, data, GL_ARRAY_BUFFER);
	}

	void BatchRenderer3D::InitBuffer(void)
	{
		m_buffer = std::make_unique <::gl::Buffer>();
	}

}