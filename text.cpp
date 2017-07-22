#include "text.h"



Text::Text(char* inText, int x, int y, int w, int h, SDL_Renderer* inRenderer, TTF_Font* inFont)
{
	text = inText;

	font = inFont;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	renderer = inRenderer;
	color = { 0 , 0 , 0 };

	surface = TTF_RenderText_Solid(font, text, color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
}

void Text::update_texture(const char* inText)
{
	text = (char*)inText;
	surface = TTF_RenderText_Solid(font, text, color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
}

Text::~Text()
{
}
