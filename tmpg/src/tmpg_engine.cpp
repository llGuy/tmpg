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

		// initialize timer
		m_timer.Start();
	}

	float TMPGEng::FPS(void)
	{
		return m_timer.FPS();
	}

	void TMPGEng::Render(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		RenderPlayers();
		RenderBullets();
		RenderPlatforms();
	}

	void TMPGEng::UpdateWin(void)
	{
		m_win.Refresh();
	}

	void TMPGEng::UpdateData(void)
	{
		float elapsed = m_timer.Elapsed();
		CheckMouseUpdates();
		CheckKeyboardUpdates(elapsed);
		m_entitiesHandler.UpdateEntities(m_physicsHandler.Gravity(), elapsed);

		m_platform.UpdateForcePoints(elapsed);
		m_timer.Reset();
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
		decltype(auto) player = m_entitiesHandler.PlayerBoundByCamera();
		if (m_inputHandler.MouseButton(GLFW_MOUSE_BUTTON_1)) m_entitiesHandler.PushBullet();

		if (m_inputHandler.MouseButton(GLFW_MOUSE_BUTTON_2)) m_platform.HandleAction(START_TERRAFORMING, player);
		else m_platform.HandleAction(END_TERRAFORMING, player);
	}

	void TMPGEng::CheckKeyboardUpdates(float elapsed)
	{
		decltype(auto) playerBoundByCamera = m_entitiesHandler.PlayerBoundByCamera();
		if (m_inputHandler.Key(GLFW_KEY_W)) playerBoundByCamera.Move(FORWARD, elapsed);
		if (m_inputHandler.Key(GLFW_KEY_A)) playerBoundByCamera.Move(LEFT, elapsed);
		if (m_inputHandler.Key(GLFW_KEY_S)) playerBoundByCamera.Move(BACKWARD, elapsed);
		if (m_inputHandler.Key(GLFW_KEY_D)) playerBoundByCamera.Move(RIGHT, elapsed);
		if (m_inputHandler.Key(GLFW_KEY_SPACE)) playerBoundByCamera.Move(JUMP, elapsed);
		if (m_inputHandler.Key(GLFW_KEY_LEFT_SHIFT)) playerBoundByCamera.Move(DOWN, elapsed);
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
		m_entitiesHandler.PushPlayer(glm::vec3(0.01f, 0.0f, 2.0f),
									 glm::vec3(1.0f, 0.01f, 1.0f));
		m_entitiesHandler.PushPlayer(glm::vec3(0.0f, 0.0f, 0.0f),
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

	void TMPGEng::RenderBullets(void)
	{
		m_sceneLayer.BindRenderer(0);

		decltype(auto) program = m_sceneLayer.ShaderProgram();
		decltype(auto) view = m_entitiesHandler.CameraViewMatrix();
		decltype(auto) projection = m_sceneLayer.ProjectionMatrix();
		decltype(auto) sunPosition = m_physicsHandler.SunPosition();

		program.UseProgram();
		for (uint32_t i = 0; i < m_entitiesHandler.NumBullets(); ++i)
		{
			// get all necessary data for draw call
			glm::vec3 modelColor(0.2f, 0.2f, 0.2f);
			glm::mat4 translation = glm::translate(m_entitiesHandler.BulletAt(i).Position());
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
	
	void TMPGEng::RenderPlayers(void)
	{
		// bind to entity model
		m_sceneLayer.BindRenderer(0);

		decltype(auto) program = m_sceneLayer.ShaderProgram();
		decltype(auto) view = m_entitiesHandler.CameraViewMatrix();
		decltype(auto) projection = m_sceneLayer.ProjectionMatrix();
		decltype(auto) sunPosition = m_physicsHandler.SunPosition();
		program.UseProgram();
		for (uint32_t i = 0; i < m_entitiesHandler.NumPlayers(); ++i)
		{
			// only render the entities that the camera can see
			if (i != m_entitiesHandler.PlayerBoundByCamera().ID())
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
		decltype(auto) view = m_entitiesHandler.CameraViewMatrix();
		decltype(auto) projection = m_sceneLayer.ProjectionMatrix();
		decltype(auto) sunPosition = m_physicsHandler.SunPosition();
		program.UseProgram();

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