#pragma once
#include"Consts.h"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include"GameObject.h"
#include"Collision.h"
#include"engine.h"



struct Framework
{
    Framework();
    ~Framework();
    void init_SDL();
    void init_imgui();
    void cleanup();
   
    void poll_events(bool& isRunning);
    void new_frame();
    void draw()
    {
        
    }
    void draw_imgui();
   
    void present()
    {

    }
    SDL_Window* pWindow;
    SDL_Renderer* pRenderer;
    int mX, mY;

    bool drawGizmos = true;
    SDL_Texture* pRenderTarget; // Render Target
    Engine _engine;
    GameObject* player;
    v4 _BgClearClr = v4(255, 0 ,255, 255);
    // things to be moved to engine class
    std::vector<GameObject> _vGameObjects;
};

