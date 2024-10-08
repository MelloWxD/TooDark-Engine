#pragma once
#include"Consts.h"


class Animation;
class Framework;
class GameObject;

#include<rapidjson/document.h>
#include<rapidjson/filereadstream.h>
#include<rapidjson/prettywriter.h>
#include<rapidjson\stringbuffer.h>

class Serializer
{
public:
	void savePrefabToJson(GameObject* pGO, const char* name);
    
	void savePrefabsToJson(const char* name);

	std::vector<GameObject> _vPrefabs;
};
class AssetManager
{
public:
	
	/// <summary>
	/// Load an image from file then store it into the map
	/// </summary>
	/// <param name="pRenderer">ptr to Renderer instance</param>
	/// <param name="TexturePath">Path to the image to load</param>
	void loadTexture(SDL_Renderer* pRenderer, const char* TexturePath, std::string TexName);
	
	void loadAnimation(SDL_Renderer* pRenderer, const char* fullAnimDirectoryPath, std::string animName);
	

	void loadAssetsFromMasterPath(std::string root_dir, Framework& fw);

	/// <summary>
	/// Retrieve texture by ID from the cached map
	/// </summary>
	/// <param name="TexID">TextureID / Name of the Texture</param>
	/// <returns>ptr to the Tex Data</returns>
	SDL_Texture* getTexture(std::string TexID)
	{
		return _texMap[TexID];
	}
	Animation& getAnim(std::string animID)
	{
		return _animationMap[animID];
	}

	std::unordered_map<std::string, SDL_Texture*> _texMap;
	std::unordered_map<std::string, Animation> _animationMap;
	Serializer _serializer;
};

