#pragma once

#include"Consts.h"
#include"engine.h"
#include"Animator.h"

// Forward declerations
class Framework;


class GameObject
{
public:
	GameObject(Framework* pFw, std::string TextureID, std::string Name = "Unnamed");
	void ChangeSprite(SDL_Texture* tx);
	void update();
	SDL_Texture* pTexImg;
	std::string _name;
	v2 position = v2(50, 50);
	v2 velocity = v2(0, 0);
	v2 Scale = v2(100, 200);
	SDL_Rect _rect = { (int) position.x,(int) position.y,(int)Scale.x,(int)Scale.y };

	int current_AnimFrame = 0;
	Animator _animator;

private:
	
	SDL_Renderer* _pRenderer;
};

