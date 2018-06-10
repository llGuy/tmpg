#include "tmpg_engine.h"

namespace tmpg {

	TMPGEng::TMPGEng(void)
		: m_win("the multiplayer game", DISPLAY_WIDTH, DISPLAY_HEIGHT),
		m_sceneLayer(glm::radians(60.0f), (float)DISPLAY_WIDTH / DISPLAY_HEIGHT, 0.01f, 1000.0f)
	{
	}

	void TMPGEng::InitData(void)
	{
		InitEntities();
		m_platform.Generate();
		InitLayers();
		Configure();
	}

	void TMPGEng::Render(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		RenderEntities();
		RenderPlatforms();
	}

	void TMPGEng::UpdateWin(void)
	{
		m_win.Refresh();
	}

	void TMPGEng::UpdateData(void)
	{
		CheckMouseUpdates();
		CheckKeyboardUpdates();
		m_entitiesHandler.UpdateEntities();

#define TIME 0.02f
		m_platform.UpdateForcePoints(TIME);
	}

	void TMPGEng::Configure(void)
	{
		m_configs.sensitivity = 0.02f;
	}

	void TMPGEng::CheckMouseUpdates(void)
	{
		// cursor movement
		if (m_inputHandler.CursorMoved())
		{
			// update camera direction
			decltype(auto) cursorPosition = m_inputHandler.CursorPosition();
			m_entitiesHandler.UpdateCameraDirection(cursorPosition, m_configs.sensitivity);
		}

		// mouse buttons
		if (m_inputHandler.MouseButton(GLFW_MOUSE_BUTTON_2)) m_platform.HandleAction(START_TERRAFORMING, m_entitiesHandler.EntityBoundByCamera());
		else m_platform.HandleAction(END_TERRAFORMING, m_entitiesHandler.EntityBoundByCamera());
	}

	void TMPGEng::CheckKeyboardUpdates(void)
	{
		// temporary
#define TIME 0.02f
		decltype(auto) entityBoundByCamera = m_entitiesHandler.EntityBoundByCamera();
		if (m_inputHandler.Key(GLFW_KEY_W)) entityBoundByCamera.Move(FORWARD, TIME);
		if (m_inputHandler.Key(GLFW_KEY_A)) entityBoundByCamera.Move(LEFT, TIME);
		if (m_inputHandler.Key(GLFW_KEY_S)) entityBoundByCamera.Move(BACKWARD, TIME);
		if (m_inputHandler.Key(GLFW_KEY_D)) entityBoundByCamera.Move(RIGHT, TIME);
		if (m_inputHandler.Key(GLFW_KEY_SPACE)) entityBoundByCamera.Move(JUMP, TIME);
		if (m_inputHandler.Key(GLFW_KEY_LEFT_SHIFT)) entityBoundByCamera.Move(DOWN, TIME);
	}

	void TMPGEng::InitWin(void)
	{
		m_win.WindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		m_win.WindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		m_win.WindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		m_win.WindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		m_win.Init();
		m_win.UserPointer(&m_inputHandler);

		m_win.KeyCallback(InputHandler::HandleKeyInput);
		m_win.MouseButtonCallback(InputHandler::HandleMouseInput);
		m_win.CursorMovementCallback(InputHandler::HandleMouseMovement);
		m_win.InputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void TMPGEng::InitEntities(void)
	{
		m_entitiesHandler.Init();
		m_entitiesHandler.PushEntity(glm::vec3(0.01f, 0.0f, 2.0f),
									 glm::vec3(1.0f, 0.01f, 1.0f));
		m_entitiesHandler.PushEntity(glm::vec3(0.0f, 0.0f, 0.0f),
									 glm::vec3(0.01f, 0.01f, -1.0f));

		// initialize camera
		m_entitiesHandler.BindCamera(0); // first entity
		// initialize camera cursor position 
		m_entitiesHandler.UpdateCameraCursorPosition(m_win.CursorPosition());
	}

	void TMPGEng::InitLayers(void)
	{
		// prepare scene layer with the entity's model
		m_sceneLayer.PushObject(m_entitiesHandler.Model3D());
		m_sceneLayer.PushObject(m_platform.Mesh());

		decltype(auto) program = m_sceneLayer.ShaderProgram();
		program.CreateShader(GL_VERTEX_SHADER, ::gl::Program::ShaderPath("scene/vsh.shader"));
		program.CreateShader(GL_GEOMETRY_SHADER, ::gl::Program::ShaderPath("scene/gsh.shader"));
		program.CreateShader(GL_FRAGMENT_SHADER, ::gl::Program::ShaderPath("scene/fsh.shader"));
		program.LinkShaders("vertex_position");
		program.GetUniformLocations("color", "model_matrix", "view_matrix", "projection_matrix", "light_position");
	}

	bool TMPGEng::Running(void) const
	{
		return m_win.Open();
	}
	
	void TMPGEng::RenderEntities(void)
	{
		// bind to entity model
		m_sceneLayer.BindRenderer(0);

		decltype(auto) program = m_sceneLayer.ShaderProgram();
		program.UseProgram();
		glm::mat4& view = m_entitiesHandler.CameraViewMatrix();
		glm::mat4& projection = m_sceneLayer.ProjectionMatrix();
		decltype(auto) sunPosition = m_physicsHandler.SunPosition();
		for (uint32_t i = 0; i < m_entitiesHandler.Size(); ++i)
		{
			// only render the entities that the camera can see
			if (i != m_entitiesHandler.EntityBoundByCamera().ID())
			{
				// get all necessary data for draw call
				glm::vec3 modelColor(0.2f, 0.2f, 0.2f);
				glm::mat4 translation = glm::translate(m_entitiesHandler[i].EyePosition());
				// prepare shader program
				program.Uniform3f(&modelColor[0], 0);
				program.UniformMat4(&translation[0][0], 1);
				program.UniformMat4(&view[0][0], 2);
				program.UniformMat4(&projection[0][0], 3);
				program.Uniform3f(&sunPosition[0], 4);
				// ready to render
				m_sceneLayer.Render(GL_TRIANGLES);
			}
		}
	}

	void TMPGEng::RenderPlatforms(void)
	{
		// bind to platform
		m_sceneLayer.BindRenderer(1);

		decltype(auto) program = m_sceneLayer.ShaderProgram();
		program.UseProgram();
		decltype(auto) view = m_entitiesHandler.CameraViewMatrix();
		decltype(auto) projection = m_sceneLayer.ProjectionMatrix();
		decltype(auto) sunPosition = m_physicsHandler.SunPosition();

		glm::vec3 modelColor(0.8f, 0.4f, 0.0f);
		glm::mat4 translation(1.0f);

		program.Uniform3f(&modelColor[0], 0);
		program.UniformMat4(&translation[0][0], 1);
		program.UniformMat4(&view[0][0], 2);
		program.UniformMat4(&projection[0][0], 3);
		program.Uniform3f(&sunPosition[0], 4);

		m_sceneLayer.Render(GL_TRIANGLES);
	}

}