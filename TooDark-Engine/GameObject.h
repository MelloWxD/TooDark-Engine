#pragma once

#include"Consts.h"
#include"engine.h"
#include"Animator.h"
#include"Collision.h"

// Forward declerations
class Framework;


class GameObject
{
public:
	GameObject(Framework* pFw, std::string TextureID, std::string Name = "Unnamed", v2 pos = v2(0));
	void ChangeSprite(SDL_Texture* tx);
	void update();
	SDL_Texture* pTexImg;
	std::string _name;
	std::string texID;
	v2 position = v2(0, 0);
	v2 velocity = v2(0, 0);
	v2 Scale = v2(100, 200);

	bool col = false;
	SDL_Rect _rect = { (int) position.x,(int) position.y,(int)Scale.x,(int)Scale.y }; // used for the drawing of the image

	Collisions::Collider* _hitbox;

	int current_AnimFrame = 0;
	Animator _animator;

private:
	
	SDL_Renderer* _pRenderer;
};

