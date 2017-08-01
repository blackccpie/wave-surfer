/*
The MIT License

Copyright (c) 2017-2017 Albert Murienne

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <chrono>

static void update(game* game, double deltaTime, const game::directions& dirs, bool &menu);
static void process_input(game* game, game::directions& dirs, SDL_DisplayMode &display, SDL_Window* window, bool &fullscreen);
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
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	// MAGIC CODE IN HERE THANKS GOODBYE
	SDL_RenderSetLogicalSize(renderer, 1024, 576);

	std::cout << window << " " << renderer << std::endl;

	game game(window, renderer);
	game.m_running = true;

	int start = SDL_GetTicks();
	int time = 0;
	int dt = 0;

	bool menu = true;
	bool fullscreen = false;

	game::directions dirs{};

	while( game.m_running )
	{
		//##### GAME LOOP #####

		start = SDL_GetTicks();

		double actualDT = dt / (float)1000;

		//std::cout << actualDT << std::endl;

		process_input(&game,dirs,display,window,fullscreen);
		update(&game, actualDT, dirs, menu);
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
	}

	return 0;
}

void update(game* game, double dt, const game::directions& dirs, bool &menu)
{
	game->update(dt, dirs, menu);
}

void process_input(game* game, game::directions& dirs, SDL_DisplayMode &display, SDL_Window* window, bool &fullscreen)
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
				game->m_running = false;
				break;
			case SDLK_LEFT:
				dirs.left = true;
				break;
			case SDLK_RIGHT:
				dirs.right = true;
				break;
			case SDLK_a:
				game->m_play = true;
				break;
			case SDLK_f:
				{
					if (fullscreen)
					{
						//Exiting fullscreen, setting a new resolution seems to be futile
						SDL_SetWindowFullscreen(window, 0);
						//SDL_SetWindowSize(win, 640, 360);
						fullscreen = false;
					}
					else
					{
						//Entering fullscreen, gets screen size, sets the game to that and then enters fullscreen
						SDL_GetCurrentDisplayMode(0, &display);
						int width = display.w;
						int height = display.h;
						SDL_SetWindowSize(window, width, height);
						SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
						fullscreen = true;
					}
				}
				break;
			default:
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				dirs.left = false;
				break;
			case SDLK_RIGHT:
				dirs.right = false;
				break;
			}
			break;
		case SDL_QUIT:
			game->m_running = false;
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

	SDL_RenderCopy(game->renderer, game->m_waves->texture, NULL, &game->m_waves->dstRect);

	for (auto& sprite : game->spriteList)
	{
		SDL_RenderCopyEx(game->renderer, sprite->texture, &sprite->srcRect, &sprite->dstRect, sprite->rotation, 0, SDL_FLIP_NONE);
	}

	for (auto& enmy : game->obSpawner->enemyList)
	{
		SDL_RenderCopy(game->renderer, enmy->texture, &enmy->srcRect, &enmy->dstRect);
	}

	SDL_RenderCopy(game->renderer, game->scoreText->texture, NULL, &game->scoreText->rect);
	SDL_RenderPresent(game->renderer);
}
