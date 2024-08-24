#include "Framework.h"
#include <imgui_impl_sdl2.h>





Framework::Framework()
{
    pRenderer = new Renderer(this);
    _engine._assetManager.loadAssetsFromMasterPath("..\\Assets\\", *this);

    _vScenes.push_back(Scene(this));
    _vScenes[_sceneIdx]._SceneGameObjects.push_back(GameObject(this, "test"));
    _vScenes[_sceneIdx]._SceneGameObjects.push_back(GameObject(this, "test"));
    player = &_vScenes[_sceneIdx]._SceneGameObjects[0];


}

Framework::~Framework()
{
    cleanup();
}



/// <summary>
/// Cleanup for framework, frees all used resources
/// </summary>
void Framework::cleanup()
{
    delete (pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
}

/// <summary>
/// Poll SDL Events for window events and input handling
/// </summary>
/// <param name="isRunning">Program Status Bool</param>
void Framework::poll_events(bool& isRunning)
{
    SDL_Event e;
    // Poll SDL events
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            isRunning = false;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.clicks == 1)
            {
                clicked = true;
            }
        }
      
        if (e.type == SDL_KEYUP) // A key is pressed
        {
             switch (e.key.keysym.sym)
            {
            case SDLK_d:
                player->velocity.x = 0;
                break;
            case SDLK_a:
                player->velocity.x = 0;
                break;
            case SDLK_w:
                player->velocity.y = 0;
                break;
            case SDLK_s:
                player->velocity.y = 0;
                break;
            }
        }
        if (e.type == SDL_KEYDOWN) // A key is pressed
        {
            if (e.key.keysym.sym == SDLK_ESCAPE) // Toggle Lock the mouse when g is pressed
            {
                if (polygonEditMode)
                {
                    polygonEditMode = false;
                    break;
                }
                isRunning = false;
                ImGui::EndFrame();
                break;
            }

            switch (e.key.keysym.sym)
            {
            case SDLK_d:
                player->velocity.x = 1;
                break;
            case SDLK_a:
                player->velocity.x = -1;
                break;
            case SDLK_w:
                player->velocity.y = -1;
                break;
            case SDLK_s:
                player->velocity.y = 1;
                break; 
            case SDLK_SPACE:
                player->_animator.playAnimOnce(1);
                break;

            }
            
        }


        ImGui_ImplSDL2_ProcessEvent(&e);
    }
    
    SDL_GetMouseState(&mX, &mY);
}



