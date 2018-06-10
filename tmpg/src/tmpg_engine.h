#ifndef _TMPG_ENGINE_H_
#define _TMPG_ENGINE_H_

#include "entities_handler.h"
#include "physics_handler.h"
#include "input_handler.h"
#include "renderer_3D.h"
#include "layer_3D.h"
#include "program.h"
#include "configs.h"
#include "window.h"

#define DISPLAY_WIDTH 2600
#define DISPLAY_HEIGHT 1200

namespace tmpg {

	class TMPGEng
	{
	public:
		TMPGEng(void);

		void Render(void);
		void InitData(void);
		void UpdateWin(void);
		void UpdateData(void);

		bool Running(void) const;
	public:
		void InitWin(void);
		void InitLayers(void);
		void InitEntities(void);
		void Configure(void);
	private:
		// render
		void RenderEntities(void);
		void CheckMouseUpdates(void);
		void CheckKeyboardUpdates(void);
	private:
		Window m_win;
		Configs m_configs;
		Layer3D m_sceneLayer;
		InputHandler m_inputHandler;
		physics::PhysicsHandler m_physicsHandler;
		EntitiesHandler m_entitiesHandler;
	};

}

#endif