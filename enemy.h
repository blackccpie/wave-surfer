#pragma once
#include "sprite.h"
class Enemy :
	public Sprite
{
public:
	Enemy();
	Enemy(int w, int h, int x, int y, SDL_Renderer* renderer, int inAnimStyle, SDL_Surface* inSurface, double inAnimDur, int inSrcWidth, double speed);
	void update(double dt, double waveVel);
	~Enemy();

	double speed;
};
