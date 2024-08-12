


#include"Framework.h"


int main(int argc, char* argv[])
{
    bool isRunning = true;
    Framework fw;
    // Create some gameobjs
    fw._vGameObjects.push_back(GameObject(fw.pRenderer, "../Assets/test.png"));
    fw._vGameObjects.push_back(GameObject(fw.pRenderer, "../Assets/transparent_test.png"));
    while (isRunning)
    {
        fw.poll_events(isRunning);

        // New frame setup
        fw.new_frame();

        SDL_SetRenderTarget(fw.pRenderer, fw.texture);


        SDL_SetRenderDrawColor(fw.pRenderer, 255, 0, 255, 255);

        SDL_RenderClear(fw.pRenderer);


        for (auto& go : fw._vGameObjects)
        {
            SDL_RenderCopy(fw.pRenderer, go.imgTex, NULL, &go._rect);
        }
       


        SDL_SetRenderTarget(fw.pRenderer, NULL);
        

        fw.draw_imgui();

        SDL_SetRenderDrawColor(fw.pRenderer, 120, 180, 255, 255);
        SDL_RenderClear(fw.pRenderer);
      
        //draw imgui here
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), fw.pRenderer);

        // Present the finished image
        SDL_RenderPresent(fw.pRenderer);


    }


   
    


    return 0;
}

