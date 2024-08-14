#pragma once
#include"Consts.h"

class GameObject;


struct Animation
{
	float speed = 1.f; // How long to wait in Seconds before switching frames
	std::string name;
	bool play = true;

	std::vector<SDL_Texture*> _vAnimationSprites;
};


struct Animator
{
	void playAnim(int animIdx);
	
	void update();
	
	GameObject* pGameObj;
	bool play = false;
	int _currentAnimIdx = 0;
	int _currentFrame = 0;
	TDark_Clock Clock;
	Animation* pCurrentAnim;
	std::vector<Animation> _vAnimations;

	bool next = false;
};

