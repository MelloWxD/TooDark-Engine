

#include <stdio.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include <SDL.H>
#include <SDL_image.h>




int main(int argc, char* argv[])
{
    bool isRunning = true;

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return -1; // Error intialising
    }
    SDL_Window* pWindow = SDL_CreateWindow("2Dark v.001", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!pWindow)
    {
        SDL_Quit();
        return -1;
    }
    // Init SDL Renderer
    SDL_Renderer* pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
    if (!pRenderer)
    {
        SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return -1;
    }
    // Create a Render Target
    SDL_Texture* texture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET, 300, 200);

    // load image
    SDL_Surface* surface = IMG_Load("../Assets/test.png");
    SDL_Texture* imgTex = SDL_CreateTextureFromSurface(pRenderer, surface);
    SDL_FreeSurface(surface);
    // Draw to render Target
    SDL_SetRenderTarget(pRenderer, texture);


    SDL_SetRenderDrawColor(pRenderer, 255, 0, 255, 255);

    SDL_RenderClear(pRenderer);

    SDL_Rect rect{ 50, 50, 100, 200 };
    SDL_RenderCopy(pRenderer, imgTex, NULL, &rect);


    SDL_SetRenderTarget(pRenderer, NULL);

    // Init imgui (Docking Branch)
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplSDL2_InitForSDLRenderer(pWindow, pRenderer);
    ImGui_ImplSDLRenderer2_Init(pRenderer);


    


    while (isRunning)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                isRunning = false;
            }
            ImGui_ImplSDL2_ProcessEvent(&e);
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();

        ImGui::Begin("Game View");
        ImGui::Image((ImTextureID)texture, ImGui::GetContentRegionAvail());
        ImGui::End();


        ImGui::Render();

        ImGui::EndFrame();
        SDL_SetRenderDrawColor(pRenderer, 120, 180, 255, 255);
        SDL_RenderClear(pRenderer);
      
        //draw imgui here
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), pRenderer);

        SDL_RenderPresent(pRenderer);


    }


    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    SDL_DestroyTexture(texture);
    ImGui::DestroyContext();
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    


    return 0;
}

