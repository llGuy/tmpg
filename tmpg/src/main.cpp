#include <iostream>
#include "tmpg_engine.h"

int32_t main(int32_t argc, char* argv[])
{
	using namespace tmpg;

	TMPGEng engine(argc, argv);

	GLFWInit();
	engine.InitWin();
	GLEWInit();

	engine.InitData();
	// need to initialize connection after the engine data is initialized
	// because the client or server may need to use the data 
	// connect to some server for game
	engine.InitConnection();

	while (engine.Running())
	{
		engine.Render();
		engine.UpdateWin();
		engine.UpdateData();
	}

	GLFWTerminate();

	std::cout << "FPS COUNT : " << engine.FPS() << std::endl;
	std::cin.get();

	return 0;
}
