#ifndef _LAYER3D_H_
#define _LAYER3D_H_

#include <vector>
#include <glm/glm.hpp>

#include "renderable_3D.h"
#include "renderer_3D.h"
#include "program.h"

namespace tmpg { 

	// layer for rendering 3D scene
	class Layer3D
	{
	public:
		Layer3D(float fov, float aspect, float near, float far);
		~Layer3D(void) = default;

		void PushObject(Renderable3D* object);
		void BindRenderer(uint32_t object);
	public:
		glm::mat4& ProjectionMatrix(void);
		::gl::Program& ShaderProgram(void);
		void Render(GLenum mode);
	private:
		std::vector<Renderable3D*> m_objects;
		::gl::Program m_program;
		Renderer3D m_renderer;

		glm::mat4 m_projectionMatrix;
	};

}	

#endif