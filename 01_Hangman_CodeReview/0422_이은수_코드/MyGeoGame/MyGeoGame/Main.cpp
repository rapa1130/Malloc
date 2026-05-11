#include "GeoGame1.h"
#include <iostream>

int main()
{
	GeoGame1 game;
	if (!game.Initialize())
	{
		std::cout << "게임앱 초기화 실패" << std::endl;
		return -1;
	}
	game.Run();
	game.Finalize();

	return 0;
}