#include <glm/gtx/transform.hpp>

#include "layer_3D.h"

namespace tmpg {

	Layer3D::Layer3D(float fov, float aspect, float near, float far)
		: m_projectionMatrix(glm::perspective(fov, aspect, near, far))
	{
	}

	void Layer3D::PushObject(Renderable3D* object)
	{
		m_objects.push_back(object);
	}

	void Layer3D::BindRenderer(uint32_t object)
	{
		m_renderer.Bind(m_objects[object]);
	}

	void Layer3D::Render(GLenum mode)
	{
		m_renderer.RenderElements(mode);
	}

	::gl::Program& Layer3D::ShaderProgram(void)
	{
		return m_program;
	}
		
	glm::mat4& Layer3D::ProjectionMatrix(void)
	{
		return m_projectionMatrix;
	}

}	