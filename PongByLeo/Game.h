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
	Vector2 mPaddlePosition;
	Vector2 mBallPosition;
	Vector2 mBallVelocity;
	Uint32 mTicksCount;
	int mPaddleDirection;

	bool InitializeWindow();
	bool InitializeRenderer();
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
};
