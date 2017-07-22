#include "enemy.h"



Enemy::Enemy()
{
}

Enemy::Enemy(int w, int h, int x, int y, SDL_Renderer* inRenderer, int inAnimStyle, SDL_Surface* inSurface, double inAnimDur, int inSrcWidth, double inSpeed)
{
	xPos = x;
	yPos = y;
	dstRect.w = w;
	dstRect.h = h;
	dstRect.x = x;
	dstRect.y = y;


	speed = inSpeed;

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
	xPos -= (waveVel * speed);

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


Enemy::~Enemy()
{
}
