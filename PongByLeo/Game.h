#pragma once
#include <SDL.h>

#include "Vector2.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	bool mIsRunning;
	Vector2 mPaddle1Position;
	Vector2 mPaddle2Position;
	Vector2 mBallPosition;
	Vector2 mBallVelocity;
	Uint32 mTicksCount;
	int mPaddle1Direction;
	int mPaddle2Direction;

	bool InitializeWindow();
	bool InitializeRenderer();
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
};
