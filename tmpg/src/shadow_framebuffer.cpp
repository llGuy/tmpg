#include "shadow_framebuffer.h"

namespace gl {

	void ShadowFramebuffer::CleanUp(void)
	{
		glDeleteFramebuffers(1, &m_id);
		glDeleteTextures(1, &m_shadowMap);
	}

	void ShadowFramebuffer::InitFramebuffer(int32_t w, int32_t h) // display height
	{
		m_id = CreateFramebuffer();
		m_shadowMap = CreateDepthBufferAttachment(m_width, m_height);
		UnbindFramebuffer(w, h);
	}

	uint32_t ShadowFramebuffer::CreateFramebuffer(void)
	{
		uint32_t i;
		glGenFramebuffers(1, &i);
		glBindFramebuffer(GL_FRAMEBUFFER, i);
		glDrawBuffer(GL_NONE);
		return i;
	}

	uint32_t ShadowFramebuffer::CreateDepthBufferAttachment(int32_t w, int32_t h)
	{
		uint32_t i;
		glGenTextures(1, &i);
		glBindTexture(GL_TEXTURE_2D, i);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, i, 0);

		return i;
	}

	void ShadowFramebuffer::Bind(GLenum framebuffer, int32_t w, int32_t h)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, w, h);
	}

	void ShadowFramebuffer::UnbindFramebuffer(int32_t w, int32_t h)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, w, h);
	}

	void ShadowFramebuffer::BindFramebuffer(void)
	{
		Bind(m_id, m_width, m_height);
	}

	uint32_t ShadowFramebuffer::ShadowMap(void)
	{
		return m_shadowMap;
	}

	uint32_t ShadowFramebuffer::FBO(void)
	{
		return m_id;
	}

}