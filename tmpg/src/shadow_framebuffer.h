#ifndef _SHADOW_FRAMEBUFFER_H_
#define _SHADOW_FRAMEBUFFER_H_

#include <stdint.h>
#include <GL/glew.h>

namespace gl {

	class ShadowFramebuffer
	{
	public:
		ShadowFramebuffer(int32_t w, int32_t h)
			: m_width(w), m_height(h)
		{
		}
		void CleanUp(void);
		void InitFramebuffer(int32_t, int32_t);
		void BindFramebuffer(void);
		void UnbindFramebuffer(int32_t, int32_t);
		void Bind(GLenum fbpoint, int32_t w, int32_t h);

		uint32_t CreateDepthBufferAttachment(int32_t, int32_t);
		uint32_t CreateFramebuffer(void);

		uint32_t ShadowMap(void);
		uint32_t FBO(void);
	private:
		uint32_t m_id;
		uint32_t m_shadowMap;

		int32_t m_width;
		int32_t m_height;
	};

}

#endif