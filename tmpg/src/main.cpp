#include <iostream>
#include "tmpg_engine.h"

int32_t main(int32_t argc, char* argv[])
{
	using namespace tmpg;

	TMPGEng engine;

	GLFWInit();
	engine.InitWin();
	GLEWInit();

	engine.InitData();

	while (engine.Running())
	{
		engine.UpdateData();
		engine.Render();
		engine.UpdateWin();
	}

	GLFWTerminate();

	return 0;
}