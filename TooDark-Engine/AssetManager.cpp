#include "AssetManager.h"
#include"Animator.h"

void AssetManager::loadTexture(SDL_Renderer* pRenderer, const char* TexturePath, std::string TexName)
{
	SDL_Surface* surface = IMG_Load(TexturePath);
	SDL_Texture* imgTex = SDL_CreateTextureFromSurface(pRenderer, surface);
	SDL_FreeSurface(surface);

	_texMap.emplace(std::pair<std::string, SDL_Texture*>(TexName, imgTex));
}

void AssetManager::loadAnimation(SDL_Renderer* pRenderer, const char* fullAnimDirectoryPath, std::string animName)
{
	Animation anim;
	anim.name = animName;
	for (const auto& entry : std::filesystem::directory_iterator(fullAnimDirectoryPath))
	{
		SDL_Surface* surface = IMG_Load(entry.path().string().c_str());
		SDL_Texture* imgTex = SDL_CreateTextureFromSurface(pRenderer, surface);
		SDL_FreeSurface(surface);
		anim._vAnimationSprites.push_back(imgTex);
	}

	_animationMap.emplace(std::pair<std::string, Animation>(animName, anim));

}