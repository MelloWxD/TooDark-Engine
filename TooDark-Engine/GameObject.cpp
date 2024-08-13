#include "GameObject.h"
#include"Framework.h"

float moveSpeed = .35f;
GameObject::GameObject(Framework* pFw, std::string TextureID)
{
	// load image
	_pRenderer = pFw->pRenderer;
	pTexImg = pFw->_engine._assetManager.getTexture(TextureID);
	_name = TextureID;
}

void GameObject::update()
{
	position.x += moveSpeed * velocity.x;
	position.y += moveSpeed * velocity.y;

	_rect = { (int)position.x,(int)position.y, (int) Scale.x,(int) Scale.y };
}
