#ifndef _BATCH_RENDERER_H_
#define _BATCH_RENDERER_H_

#include <vector>
#include <memory>

#include "vao.h"
#include "buffer.h"
#include "renderable_3D.h"

namespace tmpg {

	class BatchRenderer3D
	{
	public:
		BatchRenderer3D(uint32_t bufferMaxSize);

		void EmptyBuffer(uint32_t index);

		void PushMatrix(float* data, uint32_t index);
		void PrepareMatrixAttribute(uint32_t index);
		void Bind(Renderable3D* renderable);
		void RenderElements(GLenum mode);
		void RenderElementsInstanced(GLenum mode, uint32_t i);

		void PushBuffer(void);
	private:
		Renderable3D* m_renderable;
		// buffers will contain the model transform matrices
		std::vector<std::unique_ptr<::gl::Buffer>> m_buffers;
		std::vector<uint32_t> m_sizes;
		uint32_t m_bufferMaxSize;
	};

}

#endif