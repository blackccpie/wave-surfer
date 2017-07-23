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

#include "text.h"

text::text(char* inText, int x, int y, int w, int h, SDL_Renderer* inRenderer, TTF_Font* inFont)
	: renderer( inRenderer ), rect{x,y,w,h}, font( inFont ), m_text( inText )
{
	surface = TTF_RenderText_Solid(font, m_text, color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
}

void text::update_texture(const char* inText)
{
	m_text = (char*)inText;
	surface = TTF_RenderText_Solid(font, m_text, color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
}
