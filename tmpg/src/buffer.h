#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <GL/glew.h>
#include <stdint.h>

namespace gl {

	class Buffer
	{
	public:
		Buffer(void)
		{
			glGenBuffers(1, &m_id);
		}
		~Buffer(void) = default;

		void Destroy(void)
		{
			glDeleteBuffers(1, &m_id);
		}
	public:
		void Bind(GLenum point) const
		{
			glBindBuffer(point, m_id);
		}

		void Unbind(GLenum point) const
		{
			glBindBuffer(point, 0);
		}
	public:
		template<typename _Ty>
		void Fill(uint32_t size, _Ty* data, GLenum usage, GLenum point)
		{
			Bind(point);
			glBufferData(point, size, data, usage);
		}

		template<typename _Ty>
		void PartialFill(uint32_t offset, uint32_t size, _Ty* data, GLenum point)
		{
			Bind(point);
			glBufferSubData(point, offset, size, data);
		}
	private:
		uint32_t m_id;
	};

}

#endif