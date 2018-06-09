#ifndef _TMPG_ENGINE_H_
#define _TMPG_ENGINE_H_

#include "entities_handler.h"
#include "renderer_3D.h"
#include "program.h"
#include "window.h"

namespace tmpg {

	class TMPGEng
	{
	public:
		TMPGEng(void);

		void Render(void);
		void UpdateWin(void);
		void UpdateData(void);

		bool Running(void) const;
	public:
		void InitWin(void);
		void InitEntities(void);
	private:
		// render
		void RenderEntities(void);
	private:
		Window m_win;
		Renderer3D m_renderer3D;
		EntitiesHandler m_entitiesHandler;
	};

}

#endif
