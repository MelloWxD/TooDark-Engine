#include "AssetManager.h"
#include"Animator.h"
#include"Framework.h"
#include"GameObject.h"
#include<fstream>

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

void AssetManager::loadAssetsFromMasterPath(std::string root_dir, Framework& fw)
{
    // Create some gameobjs
    std::string path = root_dir;

    // Load Assets
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory()) // Check for Directories
        {
            // TODO: save prefix (dir name)
            for (const auto& dirEntry : std::filesystem::directory_iterator(entry.path())) // each dir in this should contain animation images
            {
                std::string animDir = path + entry.path().filename().string(); // directory of the sprite containing animations
                std::string animName = dirEntry.path().filename().string();

                if (dirEntry.is_directory()) // Checking for Animations in dirs
                {
                    animDir += "/" + dirEntry.path().filename().string(); // should be something like Knight/_Attack

                    loadAnimation(fw.pRenderer->pRenderer, animDir.c_str(), animName); // Will load all images in the file 
                }
            }
        }
        fw._engine._assetManager.loadTexture(fw.pRenderer->pRenderer, entry.path().string().c_str(), entry.path().filename().stem().string());
    }
}

void writeVec2ToJsonStringBuffer(rapidjson::PrettyWriter<rapidjson::StringBuffer>& Pwriter, v2 vec, std::string label)
{

    Pwriter.Key((label + "_x").c_str()); // Key x component
    Pwriter.Double(vec.x);
    Pwriter.Key((label + "_y").c_str()); // Key x component
    Pwriter.Double(vec.y);
}
void Serializer::savePrefabsToJson(const char* name)
{
    using namespace rapidjson;
    Document d;
    // std::ofstream write;
    StringBuffer s;
    PrettyWriter<StringBuffer> Pwriter(s);

    Pwriter.StartObject();
    Pwriter.Key("Prefab_Data"); // top label - name of prefab
    Pwriter.StartArray();
    auto& pvGOs = _vPrefabs;
    for (int i = 0; i < pvGOs.size(); ++i)
    {
        GameObject* go = &pvGOs[i];
        Pwriter.StartObject();
        Pwriter.Key("Name"); // Key the name of the prefab
        Pwriter.String(go->_name.c_str());

        writeVec2ToJsonStringBuffer(Pwriter, go->Scale, "Scale");
        writeVec2ToJsonStringBuffer(Pwriter, go->_hitbox->extents, "Hitbox_Extents");
        writeVec2ToJsonStringBuffer(Pwriter, go->_hitbox->offset, "Hitbox_Offset");
        Pwriter.EndObject();

    }

    Pwriter.EndArray();
    Pwriter.EndObject();
    std::string data = s.GetString();

    std::ofstream b_stream;
    b_stream.open(name);
    if (b_stream)
    {
        b_stream << data;
    }
    if (b_stream.good())
    {
        b_stream.close();
    }
    else
    {
        assert(false && ("Failed to load %s", name));
    
    }

    return;
}
void Serializer::savePrefabToJson(GameObject* pGO, const char* name)
{
    

  
}
