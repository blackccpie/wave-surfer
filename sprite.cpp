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

#include "sprite.h"

sprite::sprite(int w, int h, double x, double y, SDL_Renderer* inRenderer, int inAnimStyle, SDL_Surface* inSurface, double inAnimDur, int inSrcWidth)
{
	dstRect.h = h;
	dstRect.w = w;
	dstRect.x = x;
	dstRect.y = y;

	xPos = x;
	yPos = y;

	yVel = 0;

	srcWidth = inSrcWidth;

	srcRect.h = srcWidth;
	srcRect.w = srcWidth;
	srcRect.x = 0;
	srcRect.y = 0;

	renderer = inRenderer;
	surface = inSurface;

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	animAcc = 0;
	animDur = inAnimDur;
	animDir = true;
	animStyle = inAnimStyle;

	oldX = 0;
	oldY = 0;

	rotVel = 0;
	rotation = 0;
	rotAcc = 0;

	flipAcc = 0;
}

void sprite::update(double dt)
{
	if (animStyle == 0)
	{
		animatePingPong(dt);
	}
	else if (animStyle == 1)
	{
		animateForward(dt);
	}
}

void sprite::animateForward(double dt)
{
	if (animAcc > animDur)
	{
		srcRect.x = srcRect.x + srcRect.w;

		if (srcRect.x >= surface->w)
		{
			srcRect.x = 0;
		}
		animAcc = 0;
	}
	animAcc += dt;
}

void sprite::animatePingPong(double dt)
{
	if (animAcc > animDur)
	{
		if (animDir)
		{
			srcRect.x = srcRect.x + srcRect.w;

			if (srcRect.x >= surface->w)
			{
				animDir = false;
				srcRect.x = srcRect.x - (srcRect.w *2);
			}
		}
		else
		{
			srcRect.x = srcRect.x - srcRect.w;

			if (srcRect.x <= 0)
			{
				animDir = true;
			}
		}

		animAcc = 0;
	}
	animAcc += dt;
}

wave::wave(int w, int h, double x, double y, SDL_Renderer* inRenderer, int inAnimStyle, SDL_Surface* inSurface, double inAnimDur, int inSrcWidth)
{
	dstRect.h = h;
	dstRect.w = w;
	dstRect.x = x;
	dstRect.y = y;

	xPos = x;
	yPos = y;

	yVel = 0;

	srcWidth = inSrcWidth;

	srcRect.h = srcWidth;
	srcRect.w = srcWidth;
	srcRect.x = 0;
	srcRect.y = 0;

	renderer = inRenderer;
	surface = inSurface;

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	animAcc = 0;
	animDur = inAnimDur;
	animDir = true;
	animStyle = inAnimStyle;

	oldX = 0;
	oldY = 0;

	rotation = 0;

	lastInp = 0;
}

double wave::updatePos(bool left, double dt)
{
	return ( ( left ? 15000. : 0. ) * 0.0001 ) * dt;
}
