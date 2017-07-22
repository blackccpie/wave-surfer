#include "sprite.h"



Sprite::Sprite()
{
}

Sprite::Sprite(int w, int h, double x, double y, SDL_Renderer* inRenderer, int inAnimStyle, SDL_Surface* inSurface, double inAnimDur, int inSrcWidth)
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

void Sprite::update(double dt)
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


Sprite::~Sprite()
{

}

void Sprite::animateForward(double dt)
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

void Sprite::animatePingPong(double dt)
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


Wave::Wave(int w, int h, double x, double y, SDL_Renderer* inRenderer, int inAnimStyle, SDL_Surface* inSurface, double inAnimDur, int inSrcWidth)
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
