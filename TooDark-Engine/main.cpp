


#include"Framework.h"

int main(int argc, char* argv[])
{
    bool isRunning = true;
    Framework fw;
    // Create some gameobjs
    std::string path = "..\\Assets\\";

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        fw._engine._assetManager.loadTexture(fw.pRenderer, entry.path().string().c_str(), entry.path().filename().stem().string());
    }

    fw._vGameObjects.push_back(GameObject(&fw, "test"));
    fw._vGameObjects.push_back(GameObject(&fw, "amogus"));
    fw._vGameObjects.push_back(GameObject(&fw, "amogus"));
    fw.player = &fw._vGameObjects[1];
    while (isRunning)
    {
        fw.poll_events(isRunning);

        // New frame setup
        fw.new_frame();

       // Drawing goes here

        for (auto& go : fw._vGameObjects)
        {
            go.update();
            SDL_RenderCopy(fw.pRenderer, go.pTexImg, NULL, &go._rect);
            SDL_SetRenderDrawColor(fw.pRenderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(fw.pRenderer, &go._rect);
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

