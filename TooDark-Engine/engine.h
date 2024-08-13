#pragma once
#include"Consts.h"
#include <filesystem>

class GameObject;

struct AssetManager
{
	/// <summary>
	/// Load an image from file then store it into the map
	/// </summary>
	/// <param name="pRenderer">ptr to Renderer instance</param>
	/// <param name="TexturePath">Path to the image to load</param>
	void loadTexture(SDL_Renderer* pRenderer, const char* TexturePath, std::string TexName)
	{
		SDL_Surface* surface = IMG_Load(TexturePath);
		SDL_Texture* imgTex = SDL_CreateTextureFromSurface(pRenderer, surface);
		SDL_FreeSurface(surface);

		_texMap.emplace(std::pair<std::string, SDL_Texture*>(TexName, imgTex));
	}
	
	/// <summary>
	/// Retrieve texture by ID from the cached map
	/// </summary>
	/// <param name="TexID">TextureID / Name of the Texture</param>
	/// <returns>ptr to the Tex Data</returns>
	SDL_Texture* getTexture(std::string TexID)
	{
		return _texMap[TexID];
	}

	std::unordered_map<std::string, SDL_Texture*> _texMap;
};

class Engine
{
public:

	AssetManager _assetManager;

private:



};