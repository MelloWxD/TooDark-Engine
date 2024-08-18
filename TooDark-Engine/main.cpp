


#include"Framework.h"

int main(int argc, char* argv[])
{
    bool isRunning = true;
    Framework fw;
    // Create some gameobjs
    std::string path = "..\\Assets\\";

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
                    animDir +=  "/" + dirEntry.path().filename().string(); // should be something like Knight/_Attack

                    fw._engine._assetManager.loadAnimation(fw.pRenderer, animDir.c_str(), animName); // Will load all images in the file 
                }
            }
        }
        fw._engine._assetManager.loadTexture(fw.pRenderer, entry.path().string().c_str(), entry.path().filename().stem().string());
    }

    fw._vGameObjects.push_back(GameObject(&fw, "test"));
   
    fw.player = &fw._vGameObjects[0];

    fw._vGameObjects[0].position = v2(150, 0);
    // main loop
    while (isRunning)
    {
        fw.poll_events(isRunning);

        // New frame setup
        fw.new_frame();

       // Drawing goes here

        for (int x = 0; x < fw._vGameObjects.size(); ++x)
        {
            for (int y = 0; y < fw._vGameObjects.size(); ++y)
            {
                if (x == y)
                {
                    continue;
                }
                auto info = Collisions::CheckCollisions(&fw._vGameObjects[x], &fw._vGameObjects[y]);
                if (info.HasCollision)
                {
                    Collisions::ResolveSAT(info);
                    fw._vGameObjects[x].col = true;
                    fw._vGameObjects[y].col = true;
                }
                else
                {
                    fw._vGameObjects[x].col = false;
                    fw._vGameObjects[y].col = false;
                }
               
            }
        }
        for (auto& go : fw._vGameObjects)
        {
           
            go.update();
            SDL_RenderCopy(fw.pRenderer, go.pTexImg, NULL, &go._rect);
          
            if (fw.drawGizmos)
            {
                
                go._hitbox->DrawGizmo(fw.pRenderer);
              /*  SDL_SetRenderDrawColor(fw.pRenderer, 0, 0, 255, 255);
                SDL_RenderDrawPoint(fw.pRenderer, go._hitbox->centre.x, go._hitbox->centre.y);
                for (auto& v : go._hitbox->getColliderVerts())
                {
                    SDL_RenderDrawPoint(fw.pRenderer, v.x, v.y);
                }
                SDL_SetRenderDrawColor(fw.pRenderer, 255, 255, 255, 255);
                (go.col) ? SDL_SetRenderDrawColor(fw.pRenderer, 0, 255, 0, 255) : SDL_SetRenderDrawColor(fw.pRenderer, 255, 0, 0, 255);
*/


                //SDL_RenderDrawPointF(fw.pRenderer, go.position.x, go.position.y);
            }
        }
       


        SDL_SetRenderTarget(fw.pRenderer, NULL);
        // draws outside the viewport go here

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

