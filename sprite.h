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

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class sprite
{
public:

	sprite(int w, int h, double x, double y, SDL_Renderer* renderer, int animStyle, SDL_Surface* surface, double animDur, int srcWidth);
	sprite() = default;

	void update(double deltaTime);
	void animateForward(double deltaTime);
	void animatePingPong(double deltaTime);

	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	SDL_Rect srcRect, dstRect;
	double animAcc, animDur;
	int animStyle;
	bool animDir;
	double xPos, yPos, xVel, yVel;
	int srcWidth;
	double oldY, oldX;
	double rotation;
	double rotVel;
	double rotAcc;
	double flipAcc;
};

class wave : public sprite {
public:

	wave(int w, int h, double x, double y, SDL_Renderer* renderer, int animStyle, SDL_Surface* surface, double animDur, int srcWidth);

	double lastInp = 0;

	double updatePos(SDL_GameController *currentController, double dt);
};
