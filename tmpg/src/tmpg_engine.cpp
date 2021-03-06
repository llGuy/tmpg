#include "tmpg_engine.h"
#include <glm/gtx/string_cast.hpp>

auto big_or_small(void) -> void
{
	int x = 1;
	char * ptr = (char*)&x;
	if (!(*ptr)) std::cout << "big" << std::endl;
	else std::cout << "small" << std::endl;
}

namespace tmpg {

	TMPGEng::TMPGEng(int32_t argc, char** argv)
		: m_win("the multiplayer game", DISPLAY_WIDTH, DISPLAY_HEIGHT),
		m_sceneLayer(glm::perspective(glm::radians(60.0f), (float)DISPLAY_WIDTH / DISPLAY_HEIGHT, 0.01f, 1000.0f))
	{

		big_or_small();

#ifdef _WIN32

		char choice;
		std::cout << "client (c)  server (s)  offline (o) : ";
		std::cin >> choice;

#else
		char choice = argv[1][1];

#endif

		if (choice == 'c' /* for client */) m_networkHandler = new net::Client;
		else if (choice == 's' /* for server */) m_networkHandler = new net::Server;
		else if (choice == 'o' /* offline */) m_networkHandler = nullptr;
	}
	
	TMPGEng::~TMPGEng(void)
	{
#ifdef _WIN32

		net::WinSocket::WSAPICleanup();

#endif
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
		glClearColor(0.44902f, 0.747059f, 1.0f, 1.0f);
		
		if (m_networkHandler != nullptr)
		{
			if (m_networkHandler->Mode() == net::SERVER) glClearColor(0.0f, 0.3f, 1.0f, 1.0f);
		}

		glEnable(GL_DEPTH_TEST);
		RenderPlatforms();

		PushPlayersRenderer();
		PushBulletsRenderer();
		RenderEntities();
	}

	void TMPGEng::UpdateWin(void)
	{
		m_win.Refresh();
	}

	void TMPGEng::InitConnection(void)
	{
#ifdef _WIN32
		net::WinSocket::WSAPIInit();
#endif

		if (m_networkHandler != nullptr)
			m_networkHandler->Launch("146.179.209.234", "5000", m_entitiesHandler, m_inputHandler, m_physicsHandler, m_platform);
	}

	void TMPGEng::UpdateData(void)
	{
		float elapsed = m_timer.Elapsed();

		m_entitiesHandler.UpdateEntities(m_physicsHandler.Gravity(), elapsed, m_platform);
	   
		if ((m_networkHandler != nullptr && m_networkHandler->Mode() == net::CLIENT) || m_networkHandler == nullptr)
		{
			m_platform.UpdateForcePoints(elapsed);
		}
		else if (m_platform.UpdatedExternally()) m_platform.UpdateMesh();

		m_platform.UpdateProtectionPoints(elapsed, m_entitiesHandler);

		CheckMouseUpdates();
		CheckKeyboardUpdates(elapsed);
		CheckWindowResize();

		m_inputHandler.PushInput(elapsed);

		m_timer.Reset();
		m_inputHandler.CursorMoved() = false;
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

		if (m_inputHandler.MouseButton(GLFW_MOUSE_BUTTON_2))
		{
			m_platform.HandleAction(START_TERRAFORMING, player);
		}
		else m_platform.HandleAction(END_TERRAFORMING, player);
	}

	void TMPGEng::CheckKeyboardUpdates(float elapsed)
	{
		decltype(auto) playerBoundByCamera = m_entitiesHandler.PlayerBoundByCamera();
		int32_t movement = 4;

		if (m_inputHandler.Key(GLFW_KEY_W)) { playerBoundByCamera.Move(FORWARD, elapsed, m_physicsHandler.Gravity()); movement--; }
		if (m_inputHandler.Key(GLFW_KEY_A)) { playerBoundByCamera.Move(LEFT, elapsed, m_physicsHandler.Gravity()); movement--; }
		if (m_inputHandler.Key(GLFW_KEY_S)) { playerBoundByCamera.Move(BACKWARD, elapsed, m_physicsHandler.Gravity()); movement--; }
		if (m_inputHandler.Key(GLFW_KEY_D)) { playerBoundByCamera.Move(RIGHT, elapsed, m_physicsHandler.Gravity()); movement--; }

		if (movement == 4)
			playerBoundByCamera.NeutralizeMomentum();

		if (m_inputHandler.Key(GLFW_KEY_SPACE)) playerBoundByCamera.Move(JUMP, elapsed, m_physicsHandler.Gravity());
		if (m_inputHandler.Key(GLFW_KEY_T)) m_entitiesHandler.ToggleThirdPerson();
		if (m_inputHandler.Key(GLFW_KEY_C)) m_platform.HandleAction(SHIELD, playerBoundByCamera);
		if (m_inputHandler.Key(GLFW_KEY_LEFT_SHIFT)) playerBoundByCamera.Move(FALL, elapsed, m_physicsHandler.Gravity());
	}

	void TMPGEng::CheckWindowResize(void)
	{
		auto& resized = m_inputHandler.Resized();
		if (resized)
		{
			auto dimensions = m_inputHandler.WindowSize();
			glViewport(0, 0, static_cast<int32_t>(dimensions.x), static_cast<int32_t>(dimensions.y));
			m_sceneLayer.ProjectionMatrix() = glm::perspective(glm::radians(60.0f), dimensions.x / dimensions.y, 0.01f, 1000.0f);
			resized = false;
		}
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
		m_win.WindowResizeCallback(InputHandler::HandleWindowResize);
		m_win.InputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void TMPGEng::InitEntities(void)
	{
		m_entitiesHandler.Init();
		m_entitiesHandler.PushPlayer(glm::vec3(0.01f, 0.0f, 2.0f),
			glm::vec3(1.0f, 0.0f, 1.0f));

		// initialize camera
		m_entitiesHandler.BindCamera(0); // first entity
		// initialize camera cursor position 
		m_entitiesHandler.UpdateCameraCursorPosition(m_win.CursorPosition());
	}

	void TMPGEng::InitLayers(void)
	{
		m_sceneLayer.PushObject(m_platform.Mesh());
		m_sceneLayer.PushObject(m_entitiesHandler.Model3D());

		// batch renderer
		decltype(auto) renderer = m_sceneLayer.Renderer();
		for (uint32_t i = 0; i < 2; ++i)
		{
			renderer.PushBuffer();
			m_sceneLayer.BindRenderable(i);
			renderer.PrepareMatrixAttribute(i);
			renderer.EmptyBuffer(i);
		}

		decltype(auto) program = m_sceneLayer.ShaderProgram();
		program.CreateShader(GL_VERTEX_SHADER, ::gl::Program::ShaderPath("scene/vsh.shader"));
		program.CreateShader(GL_GEOMETRY_SHADER, ::gl::Program::ShaderPath("scene/gsh.shader"));
		program.CreateShader(GL_FRAGMENT_SHADER, ::gl::Program::ShaderPath("scene/fsh.shader"));
		program.LinkShaders("vertex_position", "model_matrix");
		program.GetUniformLocations("color", "view_matrix", "projection_matrix", "light_position");
	}

	bool TMPGEng::Running(void) const
	{
		return m_win.Open();
	}

	void TMPGEng::PushBulletsRenderer(void)
	{
		m_sceneLayer.BindRenderable(1);
		decltype(auto) renderer = m_sceneLayer.Renderer();

		glm::mat4 scale = glm::scale(glm::vec3(BULLET_MODEL_RADIUS));

		for (uint32_t i = 0; i < m_entitiesHandler.NumBullets(); ++i)
		{
			Bullet& bullet = m_entitiesHandler.BulletAt(i);
			// get all necessary data for draw call
			glm::mat4 rotation = glm::mat4(1.0f);
			if (!bullet.Static())
			{
				glm::vec3 r = bullet.Direction();		r.z = -1.0f; r.y = 0.0f;
				rotation = glm::rotate(bullet.Angle(m_timer.Elapsed()), r);
			}
			glm::mat4 modelMatrix = glm::translate(bullet.Position()) * rotation * scale;

			renderer.PushMatrix(&modelMatrix[0][0], 1);
		}
	}

	void TMPGEng::PushPlayersRenderer(void)
	{
		// bind to entity model
		m_sceneLayer.BindRenderable(1);
		decltype(auto) renderer = m_sceneLayer.Renderer();

		for (uint32_t i = 0; i < m_entitiesHandler.NumPlayers(); ++i)
		{
			// only render the entities that the camera can see
			if (i != m_entitiesHandler.PlayerBoundByCamera().ID() || m_entitiesHandler.ThirdPerson())
			{
				// get all necessary data for draw call
				glm::mat4 translation = glm::translate(m_entitiesHandler[i].EyePosition());
				glm::vec3 angle = m_entitiesHandler[i].Angle();
				glm::mat4 model = translation * glm::rotate(angle.y, glm::vec3(0.0f, 1.0f, 0.0f));

				renderer.PushMatrix(&model[0][0], 1);
			}
		}
	}

	void TMPGEng::RenderEntities(void)
	{
		decltype(auto) program = m_sceneLayer.ShaderProgram();
		program.UseProgram();
		decltype(auto) view = m_entitiesHandler.CameraViewMatrix();
		decltype(auto) projection = m_sceneLayer.ProjectionMatrix();
		decltype(auto) sunPosition = m_physicsHandler.SunPosition();

		glm::vec3 modelColor(0.2f, 0.2f, 0.2f);

		program.Uniform3f(&modelColor[0], 0);
		program.UniformMat4(&view[0][0], 1);
		program.UniformMat4(&projection[0][0], 2);
		program.Uniform3f(&sunPosition[0], 3);

		decltype(auto) batchRenderer = m_sceneLayer.Renderer();
		batchRenderer.RenderElementsInstanced(GL_TRIANGLES, 1);
		batchRenderer.EmptyBuffer(1);
	}

	void TMPGEng::RenderPlatforms(void)
	{
		// bind to platform
		m_sceneLayer.BindRenderable(0);

		decltype(auto) renderer = m_sceneLayer.Renderer();
		decltype(auto) program = m_sceneLayer.ShaderProgram();
		decltype(auto) view = m_entitiesHandler.CameraViewMatrix();
		decltype(auto) projection = m_sceneLayer.ProjectionMatrix();
		decltype(auto) sunPosition = m_physicsHandler.SunPosition();
		program.UseProgram();

		glm::vec3 modelColor(0.8f, 0.4f, 0.0f);
		if (m_networkHandler != nullptr)
		{
			if (m_networkHandler->Mode() == net::SERVER) modelColor = glm::vec3(0.8f, 0.0f, 0.8f);
		}

		glm::mat4 translation = glm::mat4(1.0f);

		program.Uniform3f(&modelColor[0], 0);
		program.UniformMat4(&view[0][0], 1);
		program.UniformMat4(&projection[0][0], 2);
		program.Uniform3f(&sunPosition[0], 3);

		renderer.PushMatrix(&translation[0][0], 0);
		renderer.RenderElementsInstanced(GL_TRIANGLES, 0);
		renderer.EmptyBuffer(0);
	}

}
