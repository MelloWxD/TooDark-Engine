#pragma once
#include"Consts.h"


#include"GameObject.h"
#include"Collision.h"
#include"engine.h"
#include"Scene.h"
#include"Renderer.h"


struct Framework
{
    Framework();
    ~Framework();

    void cleanup();
   
    void poll_events(bool& isRunning);

    void run(bool& run)
    {
        poll_events(run);

        pRenderer->newFrame();

        _vScenes[_sceneIdx].update_objects();
            
        pRenderer->Draw();
    }
  
    SDL_Window* pWindow;
 

    int mX, mY; // mouse coords relative to SDL window
    int mVPx, mVPy; // mouse coords relative to Render Target Viewport

    int mVPPx, mVPPy; // mouse coords relative to Render Target Viewport
    int mVPSx, mVPSy; // mouse coords relative to Render Target Viewport

    int _sceneIdx = 0;

    bool drawGizmos = true;
    bool polygonEditMode = false;
    bool clicked = false;

    
    Renderer* pRenderer; // 2Dark Renderer Class

    Engine _engine;
    GameObject* player;
    // things to be moved to engine class
    std::vector<Scene> _vScenes;
};

