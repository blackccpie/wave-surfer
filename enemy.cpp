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

#include "enemy.h"

Enemy::Enemy(int w, int h, int x, int y, SDL_Renderer* inRenderer, int inAnimStyle, SDL_Surface* inSurface, double inAnimDur, int inSrcWidth, double inSpeed)
{
	xPos = x;
	yPos = y;
	dstRect.w = w;
	dstRect.h = h;
	dstRect.x = x;
	dstRect.y = y;

	m_speed = inSpeed;

	renderer = inRenderer;
	animStyle = inAnimStyle;
	surface = inSurface;
	animDur = inAnimDur;
	animDir = true;
	animAcc = 0;

	srcWidth = inSrcWidth;

	srcRect.w = srcWidth;
	srcRect.h = srcWidth;
	srcRect.x = 0;
	srcRect.y = 0;

	texture = SDL_CreateTextureFromSurface(renderer, surface);
}

void Enemy::update(double dt, double waveVel)
{
	// NOT waveXPos, waveVelocity is needed instead
	xPos -= (waveVel * m_speed);

	dstRect.x = xPos;

	if (animStyle == 0)
	{
		animatePingPong(dt);
	}
	else if (animStyle == 1)
	{
		animateForward(dt);
	}
}
