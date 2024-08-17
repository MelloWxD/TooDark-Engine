#include "Animator.h"
#include"GameObject.h"


void Animator::playAnim(int animIdx)
{
	pCurrentAnim = &_vAnimations[animIdx];
	play = true;
}


void Animator::update()
{
	if (play)
	{
		if (Clock.tick == false)
		{
			Clock.setStartTimePointNow();
			next = false;
		}
		int f = Clock.getTimeSinceStart();
		if (f >= pCurrentAnim->speed)
		{
			_currentFrame = (_currentFrame + 1) % (pCurrentAnim->_vAnimationSprites.size());
			next = true;
			Clock.tick = false;

		}

	}
}
