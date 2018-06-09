#ifndef _RENDERER3D_H_
#define _RENDERER3D_H_

#include "renderable_3D.h"

namespace tmpg {

	class Renderer3D
	{
	public:
		void Bind(Renderable3D& obj);
		void RenderElements(GLenum mode);
	private:
		Renderable3D* m_bound;
	};

}

#endif