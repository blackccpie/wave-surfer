#include "game.h"
#include <math.h>


game::game()
{

}

game::game(SDL_Window* inWindow, SDL_Renderer* inRenderer)
{
	SDL_Log("Game initialised");
	window = inWindow;
	renderer = inRenderer;
	int* rendW = new int(0);
	int* rendH = new int(0);
	SDL_RenderGetLogicalSize(renderer, rendW, rendH);

	load_Surfaces();

	if (window != 0)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n", SDL_GetError());
	}
	else
	{
		SDL_Log("Window Initialised OK!\n");
	}

	if (renderer != 0)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n", SDL_GetError());
	}
	else
	{
		SDL_Log("Window Initialised OK!\n");
	}

	score = 0;
	startTime = SDL_GetTicks();

	obSpawner = new ObstSpawner(renderer, seagullSurf);

	Sprite* temp = new Sprite(1024, 2304, 0, -1728, renderer, -1, bgSurf, 0.033, 720);
	bg.emplace_back(temp);
	Sprite* temp2 = new Sprite(1024, 2304, 0, -4032, renderer, -1, bgSurf, 0.033, 720);
	bg.emplace_back(temp2);

	waves = new Wave(1875, 202, 0, 390, renderer, 0, seaSurf, 0.023, 64);
	waves->xPos = 0;

	sprite = new Sprite(64, 64, 32, -64, renderer, 1, playerSurf, 0.05, 64);
	spriteList.push_back(sprite);

	for (int x = 7; x > 1; x -= 2)
	{
		SDL_SetSurfaceAlphaMod(cloudSurf, (255 / 7) * x);

		Sprite* tempCloud = new Sprite(128 * (x / 2), (64 * (x / 2)), 300, (x * -50) + 300, renderer, -1, cloudSurf, 0.023, 64);
		Sprite* tempCloud2 = new Sprite(128 * (x / 2), (64 * (x / 2)), 1300, (x * -50) + 300, renderer, -1, cloudSurf, 0.023, 64);

		cloudList.push_back(tempCloud);
		cloudList.push_back(tempCloud2);
	}

	scoreText = new Text("", *rendW - 110, *rendH-60, 100, 50, renderer, scoreFont);

	for (int x = 0; x < 500; x++)
	{
		Sprite* tempPx = new Sprite(1, 1, 0, 0, renderer, -1, pixelSurf, 0.023, 1);
		pixelList.push_back(tempPx);
	}

	pressA = new Sprite(476, 64, *rendW/2 - 238, *rendH/2 - 32, renderer, -1, pressASurf, 0.021, 238);
	spriteList.push_back(pressA);
}

void game::update(double dt, SDL_GameController* currentController, bool &menu)
{
	for (auto& enemy : obSpawner->enemyList)
	{
		if (sqrt(pow((sprite->dstRect.x + (sprite->dstRect.w / 2) )  - (enemy->dstRect.x + (enemy->dstRect.w / 2)),2) + pow((sprite->dstRect.y + (sprite->dstRect.h / 2)) - (enemy->dstRect.y + (enemy->dstRect.h / 4)), 2)) < (sprite->dstRect.w / 2) + (enemy->dstRect.h / 4))
		{
			menu = true;
		}
	}

	// Score over time needs work!

	score += (dt * 10);


	double axis = 0;
	double inAxis = waves->updatePos(currentController, dt);


	//Updating position of the waves
	if (inAxis > 0.04) { inAxis = 0.04; };
	if (inAxis < -0.04) { inAxis = -0.04; };

	axis = ((inAxis + waves->lastInp * 30) / 31 + waves->xPos);
	waves->xPos = axis - 0.042;
	waves->lastInp = (inAxis + waves->lastInp * 30) / 31;

	double waveVel = (waves->oldX - waves->xPos) / dt;
	obSpawner->update(dt, waveVel / 2);

	//Dealling with spawning and menu stuff ############################################################

	if (menu == true) {
		pressA->dstRect.y = 576 / 2 - 32;
		sprite->yPos = -64;
		sprite->rotation = 0;
		sprite->yVel = 0;
		score = 0;

		bool play = SDL_GameControllerGetButton(currentController, SDL_CONTROLLER_BUTTON_A);


		if (play == true) {
			pressA->dstRect.y = 1000;
			menu = false;
			startTime = SDL_GetTicks();
			score = 0;
		}
	}
	else {
		// Rotation calculations
		double rTrig = SDL_GameControllerGetAxis(currentController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		double lTrig = SDL_GameControllerGetAxis(currentController, SDL_CONTROLLER_AXIS_TRIGGERLEFT);

		if (sprite->rotAcc < 50000 && sprite->rotAcc > -50000)
		{
			sprite->rotAcc += (rTrig - lTrig) * 0.05;

		}

		sprite->rotVel = sprite->rotAcc * dt;
		sprite->rotation += sprite->rotVel * dt;

		sprite->flipAcc += sprite->rotVel * dt;

		if (sprite->rotation > 360)
		{

			sprite->rotation -= 360;
		}
		else if (sprite->rotation < 0)
		{

			sprite->rotation += 360;
		}


		if (sprite->rotAcc > 0 && rTrig == 0)
		{
			sprite->rotAcc -= 1250;
		}
		else if (sprite->rotAcc < 0 && lTrig == 0)
		{
			sprite->rotAcc += 1250;
		}
		else
		{
			//sprite->rotAcc = 0;
		}




		float fryPos = (float)60 * cos(waves->xPos + M_PI + 1.1) + 400;

		double followGravity = sprite->yVel + 5 * dt;
		double followLine = -(sprite->yPos - fryPos);

		if (followGravity < followLine) {
			//Follow gravity

			sprite->yVel = followGravity * 1.005;


		}
		else {
			//Follow the line
			sprite->yVel = followLine * 1.005;


			if (!checkLandAngle())
			{
				menu = true;
			}
			else
			{
				int numOfFlips = abs((abs(sprite->flipAcc) + 60) / 360);

				score += 100 * numOfFlips;

				//Resets rotation on landing
				double rotation;
				if (sprite->rotation > 180) {
					rotation = sprite->rotation - 360;
				}
				else {
					rotation = sprite->rotation;
				}
				double rotAxis = ((0 + rotation * 10) / 11);
				sprite->rotation = rotAxis;
				//waves->lastInp = (0 + waves->lastInp * 30) / 31;
			}


			sprite->flipAcc = 0;


		}

		/*for (int x = 0; x < 500; x++)
		{
		pixelList[x]->dstRect.x = x + waves->xPos;
		pixelList[x]->dstRect.y = 60 * cos(waves->xPos) + 200;
		}*/

		sprite->oldY = sprite->yPos;
		sprite->yPos = sprite->yPos + sprite->yVel;



		std::string s = std::to_string((int)score);
		char const *pchar = s.c_str();
		scoreText->update_texture(pchar);
	}

	sprite->dstRect.y = sprite->yPos;



	sprite->oldX = waves->xPos;
	waves->updatePos(currentController, dt);

	sprite->update(dt);
	updateBg(dt);





	if (waves->dstRect.x < -704)
	{
		waves->xPos = 0;
	}

	waves->dstRect.x = waves->xPos * 60 -330;
	waves->oldX = waves->xPos;
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
	//This chunk of code always returns 0???
	/*double x1, x2, y1, y2;

	x1 = waves->xPos;
	x2 = waves->xPos + sprite->dstRect.w;

	y1 = (float)60 * cos(x1 + M_PI + 1.1) + 400;
	y2 = (float)60 * cos(x2 + M_PI + 1.1) + 400;

	double angle = ( atan((y1 - y2) / (x2 - x1)) ) * (180/M_PI);*/


	/*if (sprite->rotation < 1 || sprite->rotation > 359)
	{
		return true;
	}
	else if (angle > -46 && angle < 0)
	{
		if (sprite->rotation > 280 || (sprite->rotation < 50 && sprite->rotation > 0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (angle > 0 && angle < 46)
	{
		if ((sprite->rotation < 90 && sprite->rotation > 0) || sprite->rotation > 300)
		{
			return true;
		}
		else
		{
			return false;
		}
	}*/

	double waveAngle = atan(((0.5 * sin(waves->xPos + 0.2)) - (0.5 *sin(waves->xPos - 0.2)))/0.04);

	double rotation = 0;

	if (sprite->rotation > 180) {
		rotation = ((int)sprite->rotation % 360) -360;
	}
	else {
		rotation = sprite->rotation;
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

game::~game()
{

}



double Wave::updatePos(SDL_GameController *currentController, double dt) {
	return (SDL_GameControllerGetAxis(currentController, SDL_CONTROLLER_AXIS_LEFTY) * 0.0001) * dt;
}
