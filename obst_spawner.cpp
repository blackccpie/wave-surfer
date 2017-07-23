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

#include "obst_spawner.h"

static double fRand(double fmin, double fmax);

obst_spawner::obst_spawner(SDL_Renderer* inRenderer, SDL_Surface* inSurf)
{
	dtAcc = 0;
	spawnFreq = 5;

	seagullSurf = inSurf;
	renderer = inRenderer;
}

void obst_spawner::update(double dt, double waveXPos)
{
	if (dtAcc > spawnFreq)
	{
		if (enemyList.size() > 10)
		{
			delete enemyList.front();
			enemyList.erase(enemyList.begin());
		}

		Enemy* seagullSp = new Enemy(32, 32, 1100, 140 + rand() % 170 , renderer, 0, seagullSurf, 0.023, 32, fRand(1,2));
		enemyList.push_back(seagullSp);

		std::cout << "Enemy Created" << std::endl << seagullSp->m_speed << std::endl;

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
