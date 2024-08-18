#pragma once
#include"Consts.h"

class GameObject;


struct Animation
{
	int speed = 60; // How long to wait in ms before switching frames
	std::string name;
	bool play = true;

	std::vector<SDL_Texture*> _vAnimationSprites;
};


struct Animator
{
	void playAnim(int animIdx);

	void playAnimOnce(int animIdx);
	
	void update();
	
	GameObject* pGameObj;
	bool play = false;
	bool single = false;
	int _currentAnimIdx = 0;
	int _currentFrame = 0;
	TDark_Clock Clock;
	Animation* pCurrentAnim;
	Animation* pPreviousAnim;
	std::vector<Animation> _vAnimations;

	bool next = false;
};

