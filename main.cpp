#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"
#include <chrono>

static void update(game* game, double deltaTime, SDL_GameController *currentController, bool &menu);
static void process_input(game* game, SDL_DisplayMode &display, SDL_Window* window, bool &fullscreen);
static void render(game* game);

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n", SDL_GetError());
		return 1;
	}
	else
	{
		SDL_Log("SDL Initialised OK!\n");
	}

	if (TTF_Init() != 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"SDL_Init Error: %s\n", SDL_GetError());
	}
	else
	{
		SDL_Log("TTF Initialised OK!\n");
	}

	SDL_DisplayMode display;
	SDL_GetDesktopDisplayMode(0, &display);

	SDL_Window* window = SDL_CreateWindow("Wave Surfer", display.w / 4, display.h /4, display.w / 2, display.h / 2, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, NULL);

	// MAGIC CODE IN HERE THANKS GOODBYE
	SDL_RenderSetLogicalSize(renderer, 1024, 576);

	game game(window, renderer);
	game.running = true;

	/*std::chrono::high_resolution_clock::time_point prevTime = std::chrono::high_resolution_clock::now();
	double acc = 0.0;
	double dt = 0.00025;
	double t = 0.0;
	int frameCounter = 0;*/

	//Controller bindings
	SDL_GameController *controllers[8];

	//Finds all connected controllers and adds them to the above array
	int controllerNum = 0;
	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		if (i >= 8) {
			break;
		}
		if (SDL_IsGameController(i)) {
			controllers[controllerNum] = SDL_GameControllerOpen(i);
			controllerNum++;
		}
	}

	//If no controllers are detected set first controller to null pointer to allow other bits of code to function gracefully
	if (controllerNum == 0) {
		controllers[controllerNum] = nullptr;
	}

	int start = SDL_GetTicks();
	int time = 0;
	int dt = 0;

	bool menu = true;
	bool fullscreen = false;

	while (game.running)
	{
		/*auto currTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currTime - prevTime);
		double frameTimeNano = deltaTime.count();
		double frameTimeSec = frameTimeNano / 1000000000;
		prevTime = currTime;

		acc += frameTimeSec;*/

		//##### GAME LOOP #####

		start = SDL_GetTicks();

		double actualDT = dt / (float)1000;

		//std::cout << actualDT << std::endl;

		process_input(&game,display,window,fullscreen);
		update(&game, actualDT, controllers[0], menu);
		render(&game);

		time = SDL_GetTicks() - start;

		if (time < 0) continue;

		int sleepTime = 8 - time;

		if (sleepTime > 0)
		{
			SDL_Delay(sleepTime);
		}

		dt = (SDL_GetTicks() - start);



		//#####################

		/*if (frameCounter > 100)
		{
			SDL_Log("FPS: %s \n", frameTimeSec);
			frameCounter = 0;
		}
		else
		{
			frameCounter++;
		}*/
	}

	return 0;
}

void update(game* game, double dt, SDL_GameController *currentController, bool &menu)
{
	game->update(dt, currentController, menu);
}

void process_input(game* game, SDL_DisplayMode &display, SDL_Window* window, bool &fullscreen)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				game->running = false;
				break;
			default:
				break;
			}

			if (event.key.keysym.sym == SDLK_f) {
				if (fullscreen) {
					//Exiting fullscreen, setting a new resolution seems to be futile

					SDL_SetWindowFullscreen(window, 0);
					//SDL_SetWindowSize(win, 640, 360);
					fullscreen = false;
				}
				else {
					//Entering fullscreen, gets screen size, sets the game to that and then enters fullscreen
					SDL_GetCurrentDisplayMode(0, &display);
					int width = display.w;
					int height = display.h;
					SDL_SetWindowSize(window, width, height);
					SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
					fullscreen = true;
				}
			}
		}
		if (event.type == SDL_QUIT) {
			game->running = false;
			break;
		}
	}
}

void render(game* game)
{
	SDL_RenderClear(game->renderer);
	for (auto& bg : game->bg)
	{
		SDL_RenderCopy(game->renderer, bg->texture, NULL, &bg->dstRect);
	}

	for (auto& cloud : game->cloudList)
	{
		SDL_RenderCopy(game->renderer, cloud->texture, &cloud->srcRect, &cloud->dstRect);
	}

	SDL_RenderCopy(game->renderer, game->waves->texture, NULL, &game->waves->dstRect);

	for (auto& sprite : game->spriteList)
	{
		SDL_RenderCopyEx(game->renderer, sprite->texture, &sprite->srcRect, &sprite->dstRect, sprite->rotation, 0, SDL_FLIP_NONE);
	}

	for (auto& enmy : game->obSpawner->enemyList)
	{
		SDL_RenderCopy(game->renderer, enmy->texture, &enmy->srcRect, &enmy->dstRect);
	}

	SDL_RenderCopy(game->renderer, game->scoreText->texture, NULL, &game->scoreText->rect);


	/*for (auto& px : game->pixelList)
	{
		SDL_RenderCopy(game->renderer, px->texture, NULL, &px->dstRect);
	}*/

	SDL_RenderPresent(game->renderer);
}
