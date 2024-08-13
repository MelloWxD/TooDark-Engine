#pragma once

#include"Consts.h"

class Framework;

class GameObject
{
public:
	GameObject(Framework* pFw, std::string TextureID);
	void update();
	SDL_Texture* pTexImg;
	std::string _name;
	v2 position = v2(50, 50);
	v2 velocity = v2(0, 0);
	v2 Scale = v2(100, 200);
	SDL_Rect _rect = { (int) position.x,(int) position.y,(int)Scale.x,(int)Scale.y };

private:
	
	SDL_Renderer* _pRenderer;
};

