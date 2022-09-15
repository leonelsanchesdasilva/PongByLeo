#pragma once
#include "Game.h"

class PongByLeo
{
	int main(int argc, char** argv) {
		Game game;
		bool success = game.Initialize();
		if (success) {
			game.RunLoop();
		}
		game.Shutdown();
		return 0;
	}
};

