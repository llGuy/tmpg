#include "tmpg_engine.h"

namespace tmpg {

	TMPGEng::TMPGEng(void)
		: m_win("the multiplayer game", 2600, 1200)
	{
	}

	void TMPGEng::Render(void)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0f, 0.5f, 0.2f, 1.0f);
	}

	void TMPGEng::UpdateWin(void)
	{
		m_win.Refresh();
	}

	void TMPGEng::UpdateData(void)
	{
		// update data in engine
	}

	void TMPGEng::InitWin(void)
	{
		m_win.WindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		m_win.WindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		m_win.WindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		m_win.WindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_win.Init();
		m_win.UserPointer();
	}

	void TMPGEng::InitEntities(void)
	{
		m_entitiesHandler.Init();
		m_entitiesHandler.PushEntity(glm::vec3(0.01f, 0.01f, 2.0f),
									 glm::vec3(0.01f, 0.01f, -1.0f));
	}

	bool TMPGEng::Running(void) const
	{
		return m_win.Open();
	}

	void TMPGEng::RenderEntities(void)
	{
		decltype(auto) program = m_entitiesHandler.Program();
		program.UseProgram();
		m_renderer3D.Bind(m_entitiesHandler.Model3D());
		for (uint32_t ent = 0; ent < m_entitiesHandler.Size(); ++ent)
		{
			// don't render the entity that is viewing the scene
			if (ent != m_entitiesHandler.EntityBoundByCamera())
			{
				Entity& entity = m_entitiesHandler[ent];
				
				// getting uniform data
				glm::vec3 color(1.0f, 0.0f, 0.0f);

			}
		}
	}

}