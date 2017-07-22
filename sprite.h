#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Sprite
{
public:
	Sprite();
	Sprite(int w, int h, double x, double y, SDL_Renderer* renderer, int animStyle, SDL_Surface* surface, double animDur, int srcWidth);
	~Sprite();

	void update(double deltaTime);
	void animateForward(double deltaTime);
	void animatePingPong(double deltaTime);

	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Renderer* renderer;
	SDL_Rect srcRect, dstRect;
	double animAcc, animDur;
	int animStyle;
	bool animDir;
	double xPos, yPos, xVel, yVel;
	int srcWidth;
	double oldY, oldX;
	double rotation;
	double rotVel;
	double rotAcc;
	double flipAcc;
};

class Wave : public Sprite {
public:

	Wave(int w, int h, double x, double y, SDL_Renderer* renderer, int animStyle, SDL_Surface* surface, double animDur, int srcWidth);

	double lastInp = 0;

	double updatePos(SDL_GameController *currentController, double dt);
};
