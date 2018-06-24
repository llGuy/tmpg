#ifndef _TMPG_ENGINE_H_
#define _TMPG_ENGINE_H_

#include "net_handler.h"
#include "server.h"
#include "client.h"

#include "entities_handler.h"
#include "physics_handler.h"
#include "batch_renderer.h"
#include "input_handler.h"
#include "renderer_3D.h"
#include "layer_3D.h"
#include "platform.h"
#include "program.h"
#include "configs.h"
#include "window.h"
#include "timer.h"

#define DISPLAY_WIDTH 1400
#define DISPLAY_HEIGHT 900

namespace tmpg {

    class TMPGEng
    {
    public:
	TMPGEng(int32_t argc, char** argv);

	void Render(void);
	void InitData(void);
	void UpdateWin(void);
	void UpdateData(void);

	bool Running(void) const;
	float FPS(void);
    public:
	void InitWin(void);
	void InitLayers(void);
	void InitEntities(void);
	void InitConnection(void);
	void Configure(void);
    private:
	// render
	void PushBulletsRenderer(void);
	void PushPlayersRenderer(void);
	void RenderEntities(void);
	void RenderPlatforms(void);
	void CheckMouseUpdates(void);
	void CheckKeyboardUpdates(float);
    private:
	Window m_win;
	Timer m_timer;
	Configs m_configs;
	Layer3D m_sceneLayer;
	InputHandler m_inputHandler;
	physics::PhysicsHandler m_physicsHandler;
	EntitiesHandler m_entitiesHandler;

	// early stage of game, only 1 platform
	Platform m_platform;
	net::NetworkHandler* m_networkHandler;
    };

}

#endif
