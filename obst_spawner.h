#pragma once
#include <iostream>
#include "enemy.h"
#include <vector>

class ObstSpawner
{
public:
	ObstSpawner(SDL_Renderer* renderer, SDL_Surface* inSurf1);

	void update(double dt, double waveVel);

	~ObstSpawner();

	int spawnFreq;
	double dtAcc;

	std::vector<Enemy*> enemyList;
private:
	SDL_Surface* seagullSurf;
	SDL_Renderer* renderer;
};
