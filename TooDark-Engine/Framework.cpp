#include "Framework.h"
int xyzw[4] = { 0,0,0,0 };

void dragRect(GameObject& go)
{
    xyzw[0] = go._rect.x;
    xyzw[1] = go._rect.y;
    xyzw[2] = go._rect.w;
    xyzw[3] = go._rect.h;

    ImGui::DragInt4("Rect", xyzw, 1, 0, 1500, "%d");

    go._rect.x = xyzw[0];
    go._rect.y = xyzw[1];
    go._rect.w = xyzw[2];
    go._rect.h = xyzw[3];
}

void Framework::draw_imgui()
{
    ImGui::NewFrame();

    // Draws here
    ImGui::Begin("2Dark Editor", NULL, NULL);
    ImGui::SetWindowPos({0,0});
    ImGui::End();    
    ImGui::Begin("Editor Window #1");
    ImGui::End();   
    ImGui::Begin("Editor Window #2");
    ImGui::End();   
 
    ImGui::Begin("Game View");
    ImGui::Image((ImTextureID)texture, ImGui::GetContentRegionAvail());
    ImGui::End();
    ImGui::Begin("Scene Hierarchy");
    for (auto& GO : _vGameObjects)
    {
        if (ImGui::TreeNode(GO._name.c_str()))
        {
            dragRect(GO);
            ImGui::TreePop();
        }

    }

    ImGui::End();

    ImGui::Render();

    //End SDL Rendering & ImGui Input
    ImGui::EndFrame();
}