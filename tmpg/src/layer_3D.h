#ifndef _LAYER3D_H_
#define _LAYER3D_H_

#include <vector>
#include <glm/glm.hpp>

#include "batch_renderer.h"
#include "renderable_3D.h"
#include "renderer_3D.h"
#include "program.h"

namespace tmpg { 

	class Layer3D
	{
	public:
		~Layer3D(void) = default;

		Layer3D(const glm::mat4& projection)
			: m_projectionMatrix(projection),
			m_renderer(8192)
		{
		}

		void BindRenderable(uint32_t index)
		{
			m_renderer.Bind(m_objects[index]);
		}

		void PushObject(Renderable3D* object)
		{
			m_objects.push_back(object);
		}

		BatchRenderer3D& Renderer(void)
		{
			return m_renderer;
		}

		::gl::Program& ShaderProgram(void)
		{
			return m_program;
		}

		glm::mat4& ProjectionMatrix(void)
		{
			return m_projectionMatrix;
		}
	private:
		std::vector<Renderable3D*> m_objects;
		::gl::Program m_program;
		BatchRenderer3D m_renderer;

		glm::mat4 m_projectionMatrix;
	};

}	

#endif