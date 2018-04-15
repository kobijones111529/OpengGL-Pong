#include <iostream>

#include "Game/Game.h"

int main(int _argc, char* _argv[]) {
	Game game;
	
	if(!game.Init()) {
		return -1;
	}
	
	game.Run();
	game.Destroy();
	
	return 0;
}