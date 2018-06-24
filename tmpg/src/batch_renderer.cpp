#include "batch_renderer.h"

namespace tmpg {

	BatchRenderer3D::BatchRenderer3D(uint32_t bufferSize)
		: m_bufferMaxSize(bufferSize)
	{
	}

	void BatchRenderer3D::Bind(Renderable3D* renderable)
	{
		m_renderable = renderable;
	}

	void BatchRenderer3D::PrepareMatrixAttribute(uint32_t index)
	{
		::gl::VertexArray& vao = m_renderable->ModelVAO();
		uint32_t& vaoCount = m_renderable->VAOCount();
		vao.Bind();
		m_buffers[index]->Bind(GL_ARRAY_BUFFER);
		uint32_t original = vaoCount;

		for (; vaoCount < original + 4; ++vaoCount)
		{
			vao.Enable(vaoCount);
			vao.Divisor(vaoCount, 1);
			vao.VAPtr(vaoCount, 4, GL_FLOAT, 4 * 4 * sizeof(float), (void*)((vaoCount - original) * sizeof(float) * 4));
		}

		vao.Unbind();
	}

	void BatchRenderer3D::EmptyBuffer(uint32_t index)
	{
		m_sizes[index] = 0;
		m_buffers[index]->Fill<void>(m_bufferMaxSize, nullptr, GL_DYNAMIC_DRAW, GL_ARRAY_BUFFER);
	}

	void BatchRenderer3D::RenderElementsInstanced(GLenum mode, uint32_t i)
	{
		::gl::VertexArray& vao = m_renderable->ModelVAO();
		const ::gl::Buffer& buffer = m_renderable->ModelBuffer();

		vao.Bind();
		buffer.Bind(m_renderable->BindingPoint());

		glDrawElementsInstanced(mode, m_renderable->Count(), GL_UNSIGNED_SHORT, (void*)m_renderable->Offset(), m_sizes[i]);

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

	void BatchRenderer3D::PushMatrix(float* data, uint32_t index)
	{
		static constexpr uint32_t MATRIX_SIZE = 4 * 4 * sizeof(float);
		m_buffers[index]->PartialFill((m_sizes[index]++) * MATRIX_SIZE, MATRIX_SIZE, data, GL_ARRAY_BUFFER);
	}

	void BatchRenderer3D::PushBuffer(void)
	{
		m_buffers.push_back(std::make_unique<::gl::Buffer>());
		m_sizes.push_back(0);
	}

}
