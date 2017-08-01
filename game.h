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

#pragma once

#include "sprite.h"
#include "text.h"
#include "obst_spawner.h"

#include <SDL2/SDL.h>

#include <vector>
#include <iostream>
#include <string>

class game
{
public:
	struct directions
	{
		bool left;
		bool right;
	};

public:

	game(SDL_Window* window, SDL_Renderer* renderer);

	void load_Surfaces();
	void update(double deltaTime, const directions& dirs, bool &menu);
	void updateBg(double deltaTime);
	bool checkLandAngle();

	SDL_Window* window;
	SDL_Renderer* renderer;
	bool m_running;
	bool m_play;

	obst_spawner* obSpawner;
	sprite* m_sprite;
	sprite* sea;
	sprite* pressA;
	text* scoreText;
	wave* m_waves;
	double score;
	int startTime;

	std::vector<sprite*> bg;
	std::vector<sprite*> spriteList;
	std::vector<sprite*> cloudList;
	std::vector<sprite*> pixelList;
private:
	SDL_Surface* playerSurf;
	SDL_Surface* seagullSurf;
	SDL_Surface* bgSurf;
	SDL_Surface* cloudSurf;
	SDL_Surface* pixelSurf;
	SDL_Surface* seaSurf;
	SDL_Surface* pressASurf;
	TTF_Font* scoreFont;
};
