#include "entities_handler.h"

namespace tmpg {

	EntitiesHandler::EntitiesHandler(void)
		: m_entity3DModel(0.5f),
		m_entitiesProgram { GL_VERTEX_SHADER,
	                        GL_GEOMETRY_SHADER,
							GL_FRAGMENT_SHADER }
	{
	}

	void EntitiesHandler::Init(void)
	{
		m_entity3DModel.GenerateData();
		InitShaders();
	}

	void EntitiesHandler::BindCamera(uint32_t index)
	{
		m_camera.Bind(index);
	}

	void EntitiesHandler::PushEntity(const glm::vec3& p, const glm::vec3& d)
	{
		m_entities.emplace_back(p, d, m_entities.size());
	}

	Entity& EntitiesHandler::operator[](uint32_t index)
	{
		return m_entities[index];
	}

	Renderable3D& EntitiesHandler::Model3D(void) 
	{
		return m_entity3DModel;
	}

	uint32_t EntitiesHandler::Size(void) const
	{
		return m_entities.size();
	}

	uint32_t EntitiesHandler::EntityBoundByCamera(void) const
	{
		return m_camera.BoundEntity();
	}

	void EntitiesHandler::InitShaders(void)
	{
		m_entitiesProgram.Compile(
			::gl::Program<3, 4>::ShaderPath("entity/vsh.shader"),
			::gl::Program<3, 4>::ShaderPath("entity/gsh.shader"),
			::gl::Program<3, 4>::ShaderPath("entity/fsh.shader")
		);

		m_entitiesProgram.Link("vertex_position");

		m_entitiesProgram.GetUniformLocations(
			::gl::UDataLoc(::gl::udata_t::VEC3, "model_color"),
			::gl::UDataLoc(::gl::udata_t::MAT4, "matrix_model"),
			::gl::UDataLoc(::gl::udata_t::MAT4, "matrix_view"),
			::gl::UDataLoc(::gl::udata_t::MAT4, "matrix_projection")
		);
	}

	const ::gl::Program<3, 4>& EntitiesHandler::Program(void) const
	{
		return m_entitiesProgram;
	}

}