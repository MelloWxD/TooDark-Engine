#include "GameObject.h"
#include"Framework.h"

float moveSpeed = .35f;


GameObject::GameObject(Framework* pFw, std::string TextureID, std::string Name)
{
	_name = Name;
	// load image
	_pRenderer = pFw->pRenderer;
	_animator.pGameObj = this;
	_animator._vAnimations.push_back(pFw->_engine._assetManager.getAnim("_Attack"));
	pTexImg = _animator._vAnimations[_animator._currentFrame]._vAnimationSprites[_animator._currentFrame];

	_animator.playAnim(0);
}
void GameObject::ChangeSprite(SDL_Texture* tx)
{
	pTexImg = tx;
	

}

void GameObject::update()
{
	position.x += moveSpeed * velocity.x;
	position.y += moveSpeed * velocity.y;

	_rect = { (int)position.x,(int)position.y, (int) Scale.x,(int) Scale.y };

	_animator.update();
	if (_animator.next)
	{
		ChangeSprite(_animator.pCurrentAnim->_vAnimationSprites[_animator._currentFrame]);
	}
}
