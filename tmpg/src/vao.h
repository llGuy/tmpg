#ifndef _VAO_H_
#define _VAO_H_

#include <stdint.h>
#include <GL/glew.h>

namespace gl {

	class VertexArray
	{
	public:
		VertexArray(void)
		{
			glGenVertexArrays(1, &m_id);
		}

		void Bind(void) const
		{
			glBindVertexArray(m_id);
		}

		void Unbind(void) const
		{
			glBindVertexArray(0);
		}

		void Enable(uint32_t loc)
		{
			glEnableVertexAttribArray(loc);
		}

		void VAPtr(uint32_t index, uint32_t size, GLenum type, uint32_t stride, void* ptr)
		{
			glVertexAttribPointer(index, size, type, GL_FALSE, stride, ptr);
		}

		void Divisor(uint32_t index, uint32_t div)
		{
			glVertexAttribDivisor(index, div);
		}
	private:
		uint32_t m_id;
	};

}

#endif