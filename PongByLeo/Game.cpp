#include "Game.h"

const int THICKNESS = 15;
const float PADDLE_H = 100.0f;

Game::Game() : 
	mIsRunning(true), 
	mRenderer(nullptr), 
	mWindow(nullptr),
	mPaddlePosition(Vector2{ 10.0f, 768.0f / 2.0f }),
	mBallPosition(Vector2{ 1024.0f / 2.0f, 768.0f / 2.0f }),
	mPaddleDirection(0),
	mTicksCount(0)
{

}

bool Game::InitializeWindow() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	this->mWindow = SDL_CreateWindow(
		"Game programming in C++ (Chapter 1)",
		100,
		100,
		1024,
		768,
		0
	);
	if (!this->mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	return true;
}

bool Game::InitializeRenderer() {
	this->mRenderer = SDL_CreateRenderer(
		this->mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!this->mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	return true;
}

bool Game::Initialize() {
	return this->InitializeWindow() && this->InitializeRenderer();
}

void Game::Shutdown() {
	SDL_DestroyRenderer(this->mRenderer);
	SDL_DestroyWindow(this->mWindow);
	SDL_Quit();
}

void Game::RunLoop() {
	while (this->mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			this->mIsRunning = false;
			break;
		}
	}

	// Keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		this->mIsRunning = false;
	}

	mPaddleDirection = 0;
	if (state[SDL_SCANCODE_W]) {
		mPaddleDirection -= 1;
	}
	if (state[SDL_SCANCODE_S]) {
		mPaddleDirection += 1;
	}
}

void Game::UpdateGame() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), this->mTicksCount + 16));
	float deltaTime = (SDL_GetTicks() - this->mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	if (this->mPaddleDirection != 0) {
		this->mPaddlePosition.y += this->mPaddleDirection * 300.0f * deltaTime;
		// Keeping the paddle on screen
		if (this->mPaddlePosition.y < ((PADDLE_H / 2.0f) + THICKNESS)) {
			this->mPaddlePosition.y = (PADDLE_H / 2.0f) + THICKNESS;
		}
		else if (this->mPaddlePosition.y > (768.0f - (PADDLE_H / 2.0f) - THICKNESS)) {
			this->mPaddlePosition.y = 768.0f - (PADDLE_H / 2.0f) - THICKNESS;
		}
	}
}

void Game::GenerateOutput() {
	SDL_SetRenderDrawColor(
		this->mRenderer,
		0, // R
		0, // G
		255, // B
		255 // Alpha
	);

	// Back Buffer
	SDL_RenderClear(this->mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(
		this->mRenderer,
		255, // R
		255, // G
		255, // B
		255 // Alpha
	);

	// Draw top wall
	SDL_Rect wall{
		0,
		0,
		1024,
		THICKNESS
	};
	SDL_RenderFillRect(this->mRenderer, &wall);

	// Draw ball
	SDL_Rect ball{
		static_cast<int>(this->mBallPosition.x - THICKNESS / 2),
		static_cast<int>(this->mBallPosition.y - THICKNESS / 2),
		THICKNESS,
		THICKNESS
	};

	SDL_RenderFillRect(this->mRenderer, &ball);
	int paddleSize = THICKNESS * 10;

	// Draw paddle
	SDL_Rect paddle{
		static_cast<int>(this->mPaddlePosition.x - THICKNESS / 2),
		static_cast<int>(this->mPaddlePosition.y - paddleSize / 2),
		THICKNESS,
		paddleSize
	};

	SDL_RenderFillRect(this->mRenderer, &paddle);

	// Swap front and back buffers
	SDL_RenderPresent(this->mRenderer);
}