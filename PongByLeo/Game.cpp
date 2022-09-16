#include "Game.h"

const int THICKNESS = 15;
const float PADDLE_HEIGHT = 150.0f;
const float SCREEN_WIDTH = 1024.0f;
const float SCREEN_HEIGHT = 768.0f;

Game::Game() : 
	mIsRunning(true), 
	mRenderer(nullptr), 
	mWindow(nullptr),
	mPaddle1Position(Vector2{ 10.0f, SCREEN_HEIGHT / 2.0f }),
	mPaddle2Position(Vector2{ SCREEN_WIDTH - 10.0f, SCREEN_HEIGHT / 2.0f }),
	mBallPosition(Vector2{ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f }),
	mBallVelocity(Vector2{ -20.0f * 3, -23.5f * 3 }),
	mPaddle1Direction(0),
	mPaddle2Direction(0),
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

	// Player 1
	mPaddle1Direction = 0;
	if (state[SDL_SCANCODE_W]) {
		mPaddle1Direction -= 1;
	}
	if (state[SDL_SCANCODE_S]) {
		mPaddle1Direction += 1;
	}

	// Player 2
	mPaddle2Direction = 0;
	if (state[SDL_SCANCODE_UP]) {
		mPaddle2Direction -= 1;
	}
	if (state[SDL_SCANCODE_DOWN]) {
		mPaddle2Direction += 1;
	}
}

void Game::UpdateGame() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), this->mTicksCount + 16));
	float deltaTime = (SDL_GetTicks() - this->mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	// Moving left paddle
	if (this->mPaddle1Direction != 0) {
		this->mPaddle1Position.y += this->mPaddle1Direction * 300.0f * deltaTime;
		// Keeping the paddle on screen
		if (this->mPaddle1Position.y < ((PADDLE_HEIGHT / 2.0f) + THICKNESS)) {
			this->mPaddle1Position.y = (PADDLE_HEIGHT / 2.0f) + THICKNESS;
		}
		else if (this->mPaddle1Position.y > (SCREEN_HEIGHT - (PADDLE_HEIGHT / 2.0f) - THICKNESS)) {
			this->mPaddle1Position.y = SCREEN_HEIGHT - (PADDLE_HEIGHT / 2.0f) - THICKNESS;
		}
	}

	// Moving right paddle
	if (this->mPaddle2Direction != 0) {
		this->mPaddle2Position.y += this->mPaddle2Direction * 300.0f * deltaTime;
		// Keeping the paddle on screen
		if (this->mPaddle2Position.y < ((PADDLE_HEIGHT / 2.0f) + THICKNESS)) {
			this->mPaddle2Position.y = (PADDLE_HEIGHT / 2.0f) + THICKNESS;
		}
		else if (this->mPaddle2Position.y > (SCREEN_HEIGHT - (PADDLE_HEIGHT / 2.0f) - THICKNESS)) {
			this->mPaddle2Position.y = SCREEN_HEIGHT - (PADDLE_HEIGHT / 2.0f) - THICKNESS;
		}
	}

	this->mBallPosition.x += this->mBallVelocity.x * deltaTime;
	this->mBallPosition.y += this->mBallVelocity.y * deltaTime;

	// Ball touches the top
	if (this->mBallPosition.y <= THICKNESS && this->mBallVelocity.y < 0.0f) {
		this->mBallVelocity.y *= -1;
	}

	// Ball touches the bottom
	if (this->mBallPosition.y >= SCREEN_HEIGHT - THICKNESS) {
		this->mBallVelocity.y *= -1;
	}

	float diff = this->mPaddle1Position.y - this->mBallPosition.y;
	diff = (diff > 0.0f) ? diff : -diff;

	// Ball touches the left paddle
	if (
		// Difference is larger than half of paddle's height, i.e., Ball is touching the paddle
		diff <= PADDLE_HEIGHT / 2.0f &&
		// Ball is at the same x-coordinate as Paddle's 
		this->mBallPosition.x <= 25.0f && this->mBallPosition.x >= 20.0f &&
		// The ball is moving to the left
		this->mBallVelocity.x < 0.0f
		) {

		mBallVelocity.x *= -1.0f;
	}

	diff = this->mPaddle2Position.y - this->mBallPosition.y;
	diff = (diff > 0.0f) ? diff : -diff;

	// Ball touches the right paddle
	if (
		// Difference is larger than half of paddle's height, i.e., Ball is touching the paddle
		diff <= PADDLE_HEIGHT / 2.0f &&
		// Ball is at the same x-coordinate as Paddle's 
		this->mBallPosition.x >= (SCREEN_WIDTH - 25.0f) && this->mBallPosition.x <= (SCREEN_WIDTH - 20.0f) &&
		// The ball is moving to the right
		this->mBallVelocity.x > 0.0f
		) {

		mBallVelocity.x *= -1.0f;
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

	// Draw left paddle
	SDL_Rect leftPaddle{
		static_cast<int>(this->mPaddle1Position.x - THICKNESS / 2),
		static_cast<int>(this->mPaddle1Position.y - PADDLE_HEIGHT / 2),
		THICKNESS,
		PADDLE_HEIGHT
	};

	SDL_RenderFillRect(this->mRenderer, &leftPaddle);

	// Draw right paddle
	SDL_Rect rightPaddle{
		static_cast<int>(this->mPaddle2Position.x - THICKNESS / 2),
		static_cast<int>(this->mPaddle2Position.y - PADDLE_HEIGHT / 2),
		THICKNESS,
		PADDLE_HEIGHT
	};

	SDL_RenderFillRect(this->mRenderer, &rightPaddle);

	// Swap front and back buffers
	SDL_RenderPresent(this->mRenderer);
}