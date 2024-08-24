#include "Scene.h"
#include"GameObject.h"
#include"Framework.h"
#include"Collision.h"

Scene::Scene(Framework* fw, std::string scene_name)
{
	pFW = fw;
	_name = scene_name;
}

Scene::Scene(Framework* fw, std::vector<GameObject> objs, std::string scene_name)
{
	pFW = fw;
	_SceneGameObjects = objs;
	_name = scene_name;
}

void Scene::update_objects()
{
    for (int x = 0; x < _SceneGameObjects.size(); ++x)
    {
        _SceneGameObjects[x].update();
        for (int y = 0; y < _SceneGameObjects.size(); ++y)
        {
            if (x == y)
            {
                continue;
            }
            auto info = Collisions::CheckCollisions(&_SceneGameObjects[x], &_SceneGameObjects[y]);
            if (info.HasCollision)
            {
                if (info.pGameObj_l->_hitbox->_isTrigger || info.pGameObj_r->_hitbox->_isTrigger)
                {
                    continue;
                }
                Collisions::ResolveSAT(info);
                _SceneGameObjects[x].col = true;
                _SceneGameObjects[y].col = true;
            }
            else
            {
                _SceneGameObjects[x].col = false;
                _SceneGameObjects[y].col = false;
            }

        }
    }
    
}
void Scene::render_objects()
{
    for (auto& go : _SceneGameObjects)
    {
        SDL_FPoint p;
        p.x = go._hitbox->centre.x;
        p.y = go._hitbox->centre.y;
        
        
        SDL_RenderCopyExF(pFW->pRenderer->pRenderer, go.pTexImg, NULL, &go._rect, go.RotAngle, &p, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);

        if (pFW->drawGizmos)
        {

            go._hitbox->DrawGizmo(pFW->pRenderer->pRenderer);
      

        }
    }
}
void Scene::update_scene()
{
    update_objects();
    render_objects();
}
