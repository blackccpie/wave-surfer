#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Text
{
public:
	Text(char* inText, int x, int y, int w, int h, SDL_Renderer* inRenderer, TTF_Font* inFont);
	~Text();

	void update_texture(const char* inText);

	SDL_Renderer* renderer;
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect rect;
	TTF_Font* font;
	SDL_Color color;
	char* text;
};
