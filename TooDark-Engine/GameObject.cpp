#include "GameObject.h"

GameObject::GameObject(SDL_Renderer* pR, const char* TexturePath)
{
	// load image
	_pRenderer = pR;
	SDL_Surface* surface = IMG_Load(TexturePath);
	imgTex = SDL_CreateTextureFromSurface(_pRenderer, surface);
	SDL_FreeSurface(surface);
	_name = TexturePath;
}