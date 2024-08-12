#pragma once
#include"Consts.h"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include <SDL.H>
#include"GameObject.h"



struct Framework
{
	Framework()
	{
        init_SDL();
        init_imgui();
	}
    void init_SDL()
    {
        // Init SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            assert(false && " Failed to Initialize SDL!");
        }
        pWindow = SDL_CreateWindow("2Dark v.001", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN);
        if (!pWindow)
        {
            SDL_Quit();
            assert(false && " Failed to Initialize SDL Window!");
        }
        // Init SDL Renderer
        pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
        if (!pRenderer)
        {
            SDL_DestroyWindow(pWindow);
            SDL_Quit();
            assert(false && " Failed to Initialize SDL Renderer!");
        }

        // Create a Render Target
        texture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET, 300, 200);
        if (!texture)
        {
            SDL_DestroyWindow(pWindow);
            SDL_DestroyRenderer(pRenderer);
            SDL_Quit();
            assert(false && " Failed to Initialize SDL Render Target!");
        }
    }
    void init_imgui()
    {
        // Init imgui (Docking Branch)
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        auto& io = ImGui::GetIO(); (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui_ImplSDL2_InitForSDLRenderer(pWindow, pRenderer);
        ImGui_ImplSDLRenderer2_Init(pRenderer);


    }
    void cleanup()
    {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        SDL_DestroyTexture(texture);
        ImGui::DestroyContext();
        SDL_DestroyRenderer(pRenderer);
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
    }
    void poll_events(bool& isRunning)
    {
        SDL_Event e;
        // Poll SDL events
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                isRunning = false;
            }
            if (e.type == SDL_KEYDOWN) // A key is pressed
            {
                if (e.key.keysym.sym == SDLK_ESCAPE) // Toggle Lock the mouse when g is pressed
                {
                    isRunning = false;
                    ImGui::EndFrame();
                    break;
                }
            }
            ImGui_ImplSDL2_ProcessEvent(&e);
        }

    }
    void new_frame()
    {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
    }
    void draw()
    {
        
    }
    void draw_imgui();
   
    void present()
    {

    }
    SDL_Window* pWindow;
    SDL_Renderer* pRenderer;
    SDL_Texture* texture;
    // things to be moved to engine class
    std::vector<GameObject> _vGameObjects;
};

