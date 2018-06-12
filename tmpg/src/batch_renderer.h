#ifndef _BATCH_RENDERER_H_
#define _BATCH_RENDERER_H_

#include <memory>

#include "vao.h"
#include "buffer.h"
#include "renderable_3D.h"

namespace tmpg {

	class BatchRenderer3D
	{
	public:
		BatchRenderer3D(uint32_t bufferMaxSize);

		void EmptyBuffer(void);

		void PushMatrix(float* data);
		void PrepareMatrixAttribute(void);
		void Bind(Renderable3D* renderable);
		void RenderElements(GLenum mode);
		void RenderElementsInstanced(GLenum mode);

		void InitBuffer(void);
	private:
		Renderable3D* m_renderable;
		// buffers will contain the model transform matrices
		std::unique_ptr<::gl::Buffer> m_buffer;
		uint32_t m_bufferMaxSize;
		uint32_t m_size;
	};

}

#endif