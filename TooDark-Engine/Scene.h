#pragma once
#include"Consts.h";

class Framework;
class GameObject;


class Scene
{
public:

	Scene(Framework* fw, std::string scene_name = "Unnamed Scene");
	Scene(Framework* fw, std::vector<GameObject> _SceneGameObjects, std::string scene_name = "Unnamed Scene");
	
	void update_objects();

	void render_objects();

	void update_scene();

	Framework* pFW;
	std::string _name;
	std::vector<GameObject> _SceneGameObjects;
};

