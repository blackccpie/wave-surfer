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

#include "game.h"

#include <math.h>

game::game(SDL_Window* inWindow, SDL_Renderer* inRenderer)
{
	SDL_Log("Game initialised");

	window = inWindow;
	renderer = inRenderer;
	auto rendW = 0;
	auto rendH = 0;
	SDL_RenderGetLogicalSize(renderer, &rendW, &rendH);

	load_Surfaces();

	if (!window )
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Window SDL_Init Error: %s\n", SDL_GetError());
	}
	else
	{
		SDL_Log("Window Initialised OK!\n");
	}

	if (!renderer)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Renderer SDL_Init Error: %s\n", SDL_GetError());
	}
	else
	{
		SDL_Log("Renderer Initialised OK!\n");
	}

	score = 0;
	startTime = SDL_GetTicks();

	obSpawner = new obst_spawner(renderer, seagullSurf);

	sprite* temp = new sprite(1024, 2304, 0, -1728, renderer, -1, bgSurf, 0.033, 720);
	bg.emplace_back(temp);
	sprite* temp2 = new sprite(1024, 2304, 0, -4032, renderer, -1, bgSurf, 0.033, 720);
	bg.emplace_back(temp2);

	m_waves = new wave(1875, 202, 0, 390, renderer, 0, seaSurf, 0.023, 64);
	m_waves->xPos = 0;

	m_sprite = new sprite(64, 64, 32, -64, renderer, 1, playerSurf, 0.05, 64);
	spriteList.push_back(m_sprite);

	for (int x = 7; x > 1; x -= 2)
	{
		SDL_SetSurfaceAlphaMod(cloudSurf, (255 / 7) * x);

		sprite* tempCloud = new sprite(128 * (x / 2), (64 * (x / 2)), 300, (x * -50) + 300, renderer, -1, cloudSurf, 0.023, 64);
		sprite* tempCloud2 = new sprite(128 * (x / 2), (64 * (x / 2)), 1300, (x * -50) + 300, renderer, -1, cloudSurf, 0.023, 64);

		cloudList.push_back(tempCloud);
		cloudList.push_back(tempCloud2);
	}

	scoreText = new text("", rendW - 110, rendH-60, 100, 50, renderer, scoreFont);

	for (int x = 0; x < 500; x++)
	{
		sprite* tempPx = new sprite(1, 1, 0, 0, renderer, -1, pixelSurf, 0.023, 1);
		pixelList.push_back(tempPx);
	}

	pressA = new sprite(476, 64, rendW/2 - 238, rendH/2 - 32, renderer, -1, pressASurf, 0.021, 238);
	spriteList.push_back(pressA);
}

void game::update(double dt, SDL_GameController* currentController, bool &menu)
{
	for (auto& enemy : obSpawner->enemyList)
	{
		if (sqrt(pow((m_sprite->dstRect.x + (m_sprite->dstRect.w / 2) )  - (enemy->dstRect.x + (enemy->dstRect.w / 2)),2)
			+ pow((m_sprite->dstRect.y + (m_sprite->dstRect.h / 2)) - (enemy->dstRect.y + (enemy->dstRect.h / 4)), 2)) < (m_sprite->dstRect.w / 2) + (enemy->dstRect.h / 4))
		{
			menu = true;
		}
	}

	// Score over time needs work!

	score += (dt * 10);


	double axis = 0;
	double inAxis = m_waves->updatePos(currentController, dt);


	//Updating position of the waves
	if (inAxis > 0.04) { inAxis = 0.04; };
	if (inAxis < -0.04) { inAxis = -0.04; };

	axis = ((inAxis + m_waves->lastInp * 30) / 31 + m_waves->xPos);
	m_waves->xPos = axis - 0.042;
	m_waves->lastInp = (inAxis + m_waves->lastInp * 30) / 31;

	double waveVel = (m_waves->oldX - m_waves->xPos) / dt;
	obSpawner->update(dt, waveVel / 2);

	//Dealling with spawning and menu stuff ############################################################

	if ( menu )
	{
		pressA->dstRect.y = 576 / 2 - 32;
		m_sprite->yPos = -64;
		m_sprite->rotation = 0;
		m_sprite->yVel = 0;
		score = 0;

		if ( m_play == true )
		{
			pressA->dstRect.y = 1000;
			menu = false;
			startTime = SDL_GetTicks();
			score = 0;
		}
	}
	else
	{
		// Rotation calculations
		double rTrig = SDL_GameControllerGetAxis(currentController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		double lTrig = SDL_GameControllerGetAxis(currentController, SDL_CONTROLLER_AXIS_TRIGGERLEFT);

		if (m_sprite->rotAcc < 50000 && m_sprite->rotAcc > -50000)
		{
			m_sprite->rotAcc += (rTrig - lTrig) * 0.05;

		}

		m_sprite->rotVel = m_sprite->rotAcc * dt;
		m_sprite->rotation += m_sprite->rotVel * dt;

		m_sprite->flipAcc += m_sprite->rotVel * dt;

		if (m_sprite->rotation > 360)
		{
			m_sprite->rotation -= 360;
		}
		else if (m_sprite->rotation < 0)
		{
			m_sprite->rotation += 360;
		}

		if (m_sprite->rotAcc > 0 && rTrig == 0)
		{
			m_sprite->rotAcc -= 1250;
		}
		else if (m_sprite->rotAcc < 0 && lTrig == 0)
		{
			m_sprite->rotAcc += 1250;
		}
		else
		{
			//sprite->rotAcc = 0;
		}

		float fryPos = (float)60 * cos(m_waves->xPos + M_PI + 1.1) + 400;

		double followGravity = m_sprite->yVel + 5 * dt;
		double followLine = -(m_sprite->yPos - fryPos);

		if (followGravity < followLine) {
			//Follow gravity

			m_sprite->yVel = followGravity * 1.005;


		}
		else {
			//Follow the line
			m_sprite->yVel = followLine * 1.005;


			if (!checkLandAngle())
			{
				menu = true;
			}
			else
			{
				int numOfFlips = abs((abs(m_sprite->flipAcc) + 60) / 360);

				score += 100 * numOfFlips;

				//Resets rotation on landing
				double rotation;
				if (m_sprite->rotation > 180) {
					rotation = m_sprite->rotation - 360;
				}
				else {
					rotation = m_sprite->rotation;
				}
				double rotAxis = ((0 + rotation * 10) / 11);
				m_sprite->rotation = rotAxis;
				//waves->lastInp = (0 + waves->lastInp * 30) / 31;
			}

			m_sprite->flipAcc = 0;
		}

		m_sprite->oldY = m_sprite->yPos;
		m_sprite->yPos = m_sprite->yPos + m_sprite->yVel;

		std::string s = std::to_string((int)score);
		char const *pchar = s.c_str();
		scoreText->update_texture(pchar);
	}

	m_sprite->dstRect.y = m_sprite->yPos;

	m_sprite->oldX = m_waves->xPos;
	m_waves->updatePos(currentController, dt);

	m_sprite->update(dt);
	updateBg(dt);

	if (m_waves->dstRect.x < -704)
	{
		m_waves->xPos = 0;
	}

	m_waves->dstRect.x = m_waves->xPos * 60 -330;
	m_waves->oldX = m_waves->xPos;
}

void game::updateBg(double dt)
{
	for (auto& back : bg)
	{
		if (back->animAcc > back->animDur)
		{
			back->dstRect.y += 1;

			if (back->dstRect.y >= 576)
			{
				back->dstRect.y = -4032;
			}

			back->animAcc = 0;
		}
		back->animAcc += dt;
	}

	for (auto& cloud : cloudList)
	{
		if (cloud->animAcc > cloud->animDur)
		{
			cloud->dstRect.x -= 10 / (cloud->dstRect.w / 32) + cloud->xVel;

			if (cloud->dstRect.x <= -400)
			{
				cloud->dstRect.x = 1200;
				cloud->xVel = rand() % 5 + 1;
				cloud->dstRect.y = ((cloud->dstRect.h / 32) * -50) + rand() % 200 + 100;
			}

			cloud->animAcc = 0;
		}
		cloud->animAcc += dt;
	}
}

void game::load_Surfaces()
{
	playerSurf = IMG_Load("resources/jesus.png");
	if (!playerSurf) {
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
	}

	seagullSurf = IMG_Load("resources/seagull.png");
	if (!seagullSurf) {
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
	}

	bgSurf = IMG_Load("resources/background.png");
	if (!bgSurf) {
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
	}

	cloudSurf = IMG_Load("resources/cloud.png");
	if (!cloudSurf) {
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
	}

	pixelSurf = IMG_Load("resources/pixel.png");
	if (!pixelSurf) {
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
	}

	seaSurf = IMG_Load("resources/sea.png");
	if (!seaSurf) {
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
	}

	pressASurf = IMG_Load("resources/PressAContinue.png");
	if (!pressASurf) {
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
	}

	scoreFont = TTF_OpenFont("resources/true-crimes.ttf", 28);
	if (!scoreFont)
	{
		SDL_Log("TTF_OpenFont: %s\n", TTF_GetError());
	}
}

bool game::checkLandAngle()
{
	double waveAngle = atan(((0.5 * sin(m_waves->xPos + 0.2)) - (0.5 *sin(m_waves->xPos - 0.2)))/0.04);

	double rotation = 0;

	if (m_sprite->rotation > 180) {
		rotation = ((int)m_sprite->rotation % 360) -360;
	}
	else {
		rotation = m_sprite->rotation;
	}

	if (waveAngle * 180 / M_PI > 76) {
		if (rotation > -90 && rotation < 55) {
			return true;
		}
		else {
			return false;
		}
	}
	if (waveAngle * 180 / M_PI < -76) {
		if (rotation > -100 && rotation < 90) {
			return true;
		}
		else {
			return false;
		}
	}

	if (rotation > -90 && rotation < 90) {
		return true;
	}
	else {
		return false;
	}

	return false;
}
