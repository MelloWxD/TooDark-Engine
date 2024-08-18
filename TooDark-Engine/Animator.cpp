#include "Animator.h"
#include"GameObject.h"


void Animator::playAnim(int animIdx)
{
	pPreviousAnim = pCurrentAnim;
	pCurrentAnim = &_vAnimations[animIdx];
	play = true;
}

void Animator::playAnimOnce(int animIdx)
{
	if (single)
	{
		return; // stop spamming the animation queue
	}
	pPreviousAnim = pCurrentAnim;
	pCurrentAnim = &_vAnimations[animIdx];
	single = true;
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
			if (_currentFrame == pCurrentAnim->_vAnimationSprites.size()-1 && single)
			{
				single = false;
				if (pPreviousAnim != nullptr)
				{
					pCurrentAnim = pPreviousAnim;
					_currentFrame = 0;
				}
			}

			next = true;
			Clock.tick = false;

		}

	}
}
