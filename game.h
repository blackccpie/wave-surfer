#pragma once

#include <SDL2/SDL.h>
#include "sprite.h"
#include "text.h"
#include "obst_spawner.h"
#include <vector>
#include <iostream>
#include <string>

class game
{
public:
	game();
	game(SDL_Window* window, SDL_Renderer* renderer);
	~game();

	void load_Surfaces();
	void update(double deltaTime, SDL_GameController *currentController, bool &menu);
	void updateBg(double deltaTime);
	bool checkLandAngle();

	SDL_Window* window;
	SDL_Renderer* renderer;
	bool running;

	ObstSpawner* obSpawner;
	Sprite* sprite;
	Sprite* sea;
	Sprite* pressA;
	Text* scoreText;
	Wave* waves;
	double score;
	int startTime;

	std::vector<Sprite*> bg;
	std::vector<Sprite*> spriteList;
	std::vector<Sprite*> cloudList;
	std::vector<Sprite*> pixelList;
private:
	SDL_Surface* playerSurf;
	SDL_Surface* seagullSurf;
	SDL_Surface* bgSurf;
	SDL_Surface* cloudSurf;
	SDL_Surface* pixelSurf;
	SDL_Surface* seaSurf;
	SDL_Surface* pressASurf;
	TTF_Font* scoreFont;

};
