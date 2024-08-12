#pragma once
#include <SDL.H>
#include <SDL_image.h>
#include"Consts.h"


class GameObject
{
public:
	GameObject(SDL_Renderer* pR, const char* TexturePath);
	SDL_Texture* imgTex;
	SDL_Rect _rect = { 50, 50, 100, 200 };
	std::string _name;

private:
	
	SDL_Renderer* _pRenderer;
};

