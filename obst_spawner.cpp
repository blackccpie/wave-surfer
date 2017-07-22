#include "obst_spawner.h"

static double fRand(double fmin, double fmax);

ObstSpawner::ObstSpawner(SDL_Renderer* inRenderer, SDL_Surface* inSurf)
{
	dtAcc = 0;
	spawnFreq = 5;

	seagullSurf = inSurf;
	renderer = inRenderer;

}


ObstSpawner::~ObstSpawner()
{

}

void ObstSpawner::update(double dt, double waveXPos)
{
	if (dtAcc > spawnFreq)
	{
		if (enemyList.size() > 10)
		{
			delete enemyList.front();
			enemyList.erase(enemyList.begin());
		}

		double f = (double)rand() / 2;



		Enemy* seagullSp = new Enemy(32, 32, 1100, 140 + rand() % 170 , renderer, 0, seagullSurf, 0.023, 32, fRand(1,2));
		enemyList.push_back(seagullSp);

		std::cout << "Enemy Created" << std::endl << seagullSp->speed << std::endl;

		if (spawnFreq > 5)
		{
			spawnFreq *= 0.98;
		}
		dtAcc = 0;
	}
	dtAcc += dt;

	for (auto& enmy : enemyList)
	{
		enmy->update(dt, waveXPos);
	}
}

double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}
