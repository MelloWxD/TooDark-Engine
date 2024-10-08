#include "Renderer.h"
#include"Framework.h"
#include"GameObject.h"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>


int xyzw[4] = { 0,0,0,0 };
int ex[2] = { 0,0 };
int of[2] = { 0,0 };
int t[2] = { 0,0 };
int imguiTempBuff[64] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
int arr[4] = { 0,0,0,0 };
float xyzwf[4] = { 0,0,0,0 };



    void Spacing(int space)
    {
        for (int x = 0; x < space; ++x)
        {
            ImGui::Spacing();
        }
    }
    void dragVec2(const char* Label, v2& v, int speed, int min, int max)
    {
        xyzw[0] = v.x;
        xyzw[1] = v.y;


        ImGui::DragInt2(Label, xyzw, speed, min, max);

        v.x = xyzw[0];
        v.y = xyzw[1];
    }
    void dragVec2f(const char* Label, v2& v, float speed, float min, float max)
    {
        xyzwf[0] = v.x;
        xyzwf[1] = v.y;


        ImGui::DragFloat2(Label, xyzwf, speed, min, max);

        v.x = xyzwf[0];
        v.y = xyzwf[1];
    }
    void dragVec4(const char* Label, v4& v, int speed, int min, int max)
    {
        xyzw[0] = v.x;
        xyzw[1] = v.y;
        xyzw[2] = v.z;
        xyzw[3] = v.w;

        ImGui::DragInt4(Label, xyzw, speed, min, max);

        v.x = xyzw[0];
        v.y = xyzw[1];
        v.z = xyzw[2];
        v.w = xyzw[3];
    }
    void dragImVec2(const char* Label, ImVec2& v, int speed, int min, int max)
    {
        xyzwf[0] = v.x;
        xyzwf[1] = v.y;


        ImGui::DragFloat2(Label, xyzwf, speed, min, max);

        v.x = xyzwf[0];
        v.y = xyzwf[1];
    }
    void dragImVec4(const char* Label, ImVec4& v, int speed, int min, int max)
    {
        xyzwf[0] = v.x;
        xyzwf[1] = v.y;
        xyzwf[2] = v.z;
        xyzwf[3] = v.w;


        ImGui::DragFloat4(Label, xyzwf, speed, min, max);

        v.x = xyzwf[0];
        v.y = xyzwf[1];
        v.z = xyzwf[2];
        v.w = xyzwf[3];
    }
    void dragTransform(GameObject& go)
    {


        dragVec2f("Position", go.position, .001f, -FLT_MAX, FLT_MAX);
        dragVec2f("Scale", go.Scale, 0.01f, -FLT_MAX, FLT_MAX);
        ImGui::DragFloat("Rotation", &go.RotAngle, 0.01f, -360, 360, "%.3f");


    }
    void dragAABBHitbox(GameObject& go)
    {
        dragVec2f("Extents", go._hitbox->extents, 0.05f, -FLT_MAX, FLT_MAX);

        dragVec2f("Offset", go._hitbox->offset, 0.05f, -FLT_MAX, FLT_MAX);


    }
    void dragTriangleHitbox(GameObject& go)
    {
        auto v = { go._hitbox->t1, go._hitbox->t2, go._hitbox->t3 };
        int idx = 0;

        for (auto p : v) // for each triangle point
        {
            imguiTempBuff[idx] = p.x;
            imguiTempBuff[idx + 1] = p.y;
            idx += 2;
        }
        xyzw[0] = go._hitbox->offset.x;
        xyzw[1] = go._hitbox->offset.y;
        // Each point will be lined up [x1, y1, x2, y2 ... ]
        ImGui::DragInt2("Offset", xyzw, 1, -1000, 1500, "%d");
        Spacing(2);

        ImGui::DragInt2("Point #1", imguiTempBuff, 1, -1000, 1500, "%d");
        Spacing(2);
        ImGui::DragInt2("Point #2", &imguiTempBuff[2], 1, -1000, 1500, "%d");
        Spacing(2);
        ImGui::DragInt2("Point #3", &imguiTempBuff[4], 1, -1000, 1500, "%d");

        go._hitbox->t1.x = imguiTempBuff[0];
        go._hitbox->t1.y = imguiTempBuff[1];

        go._hitbox->t2.x = imguiTempBuff[2];
        go._hitbox->t2.y = imguiTempBuff[3];

        go._hitbox->t3.x = imguiTempBuff[4];
        go._hitbox->t3.y = imguiTempBuff[5];


        go._hitbox->offset = v2(xyzw[0], xyzw[1]);

    }
    void dragPolygonHitbox(GameObject& go)
    {
        auto v = go._hitbox->getColliderVerts();
        int idx = 0;
        for (auto p : v) // for each vertex of the polygon
        {
            imguiTempBuff[idx] = p.x;
            imguiTempBuff[idx + 1] = p.y; // line them up in [x1,y1, x2, y2 ... ] format
            idx += 2;
        }

        for (int i = 0; i < v.size() / 2; i += 2)
        {
            std::string s = "Point #" + std::to_string(i);
            ImGui::DragInt2(s.c_str(), &imguiTempBuff[i], 1, -1000, 1500);


        }


    }




Renderer::Renderer(Framework* pFrameowrk)
{
    pFw = pFrameowrk;
	init_SDL();
    init_ImGui();
}

void Renderer::init_SDL()
{ 
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        assert(false && " Failed to Initialize SDL!");
    }
    pFw->pWindow = SDL_CreateWindow("2Dark v.001", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 2560, 1440, SDL_WINDOW_SHOWN);
    if (!pFw->pWindow)
    {
        SDL_Quit();
        assert(false && " Failed to Initialize SDL Window!");
    }
    // Init SDL Renderer
    pRenderer = SDL_CreateRenderer(pFw->pWindow, -1, 0);
    if (!pRenderer)
    {
        SDL_DestroyWindow(pFw->pWindow);
        SDL_Quit();
        assert(false && " Failed to Initialize SDL Renderer!");
    }

    // Create a Render Target


    pRenderTarget = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);
    if (!pRenderTarget)
    {
        SDL_DestroyWindow(pFw->pWindow);
        SDL_DestroyRenderer(pRenderer);
        SDL_Quit();
        assert(false && " Failed to Initialize SDL Render Target!");
    }

    pRaycastRenderTarget = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);
    if (!pRenderTarget)
    {
        SDL_DestroyWindow(pFw->pWindow);
        SDL_DestroyRenderer(pRenderer);
        SDL_Quit();
        assert(false && " Failed to Initialize SDL Render Target!");
    }
}
void setImguiSkin()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(8.00f, 8.00f);
    style.FramePadding = ImVec2(5.00f, 2.00f);
    style.CellPadding = ImVec2(6.00f, 6.00f);
    style.ItemSpacing = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
    style.IndentSpacing = 25;
    style.ScrollbarSize = 15;
    style.GrabMinSize = 10;
    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = 1;
    style.TabBorderSize = 1;
    style.WindowRounding = 7;
    style.ChildRounding = 4;
    style.FrameRounding = 3;
    style.PopupRounding = 4;
    style.ScrollbarRounding = 9;
    style.GrabRounding = 3;
    style.LogSliderDeadzone = 4;
    style.TabRounding = 4;
}

void Renderer::init_ImGui()
{
    // Init imgui (Docking Branch)
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplSDL2_InitForSDLRenderer(pFw->pWindow, pRenderer);
    ImGui_ImplSDLRenderer2_Init(pRenderer);

    setImguiSkin();
}

void Renderer::newFrame()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    SDL_SetRenderTarget(pRenderer, pRenderTarget);


    SDL_SetRenderDrawColor(pRenderer, _BgClearClr[0], _BgClearClr[1], _BgClearClr[2], _BgClearClr[3]);

    SDL_RenderClear(pRenderer);
}

void Renderer::drawImGui()
{
    ImGui::NewFrame();


    auto& _vScenes = pFw->_vScenes;
    auto& _sceneIdx = pFw->_sceneIdx;
    // draw File menu bar
    {
        ImGui::BeginMainMenuBar();

        if (ImGui::BeginMenu("Scene"))
        {
            if (ImGui::MenuItem("New GameObject", "CTRL+N"))
            {
                _vScenes[_sceneIdx]._SceneGameObjects.push_back(GameObject(pFw, "test", "New Object", v2(50, -50)));
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Prefabs"))
        {
            if (ImGui::MenuItem("Save Prefabs to Json"))
            {
                // Name Editor
                {
                    static std::string password = "example.json";
                    ImGui::InputText("Name", (char*)password.c_str(), IM_ARRAYSIZE(password.c_str()));
                    ImGui::SameLine();
                    if (ImGui::Button("Confirm"))
                    {

                        // GO._name = password;
                    }
                }

                pFw->_engine._assetManager._serializer.savePrefabsToJson("Prefabs.json");
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

    }

    ImGui::Begin("Editor Settings");
    {
        dragVec4("Background Clear Colour", _BgClearClr, 1, 0, 255);
        ImGui::Checkbox("Toggle Gizmos", &drawGizmos);
        ImGui::Text("Style Settings");

    }
    ImGui::End();


    ImGui::Begin("Editor Window #2");
    

    v2 padding;
    padding.x = ImGui::GetContentRegionMax().x - ImGui::GetContentRegionAvail().x;
    padding.y = (ImGui::GetContentRegionMax().y - ImGui::GetContentRegionAvail().y) / 2;

   // ImGui::Text("Mouse Viewport Pos - %i,%i", mVPx, mVPy);
    ImGui::Text("Viewport Window Padding - %.3f,%.3f", padding.x, padding.y);




    static int selectedObjId = -1; // for inspector game object selection

    ImGui::End();

    ImGui::Begin("Game View");

    auto& _vGameObjects = _vScenes[_sceneIdx]._SceneGameObjects;


    ImGui::Image((ImTextureID)pRenderTarget, ImGui::GetContentRegionMax());

    ImGui::End();

    ImGui::Begin("Scene Hierarchy");
    {
        ImGui::Text("Scene Objects");
        for (int n = 0; n < _vGameObjects.size(); n++)
        {
            std::string lbl = std::to_string(n) + " - " + _vGameObjects[n]._name;

            if (ImGui::Selectable(lbl.c_str(), selectedObjId == n))
                selectedObjId = n;
        }
    }

    // Edit any selected item
    ImGui::Begin("Inspector", NULL, NULL);
    {
        if (selectedObjId >= 0) // Object chosen
        {
            auto& GO = _vGameObjects[selectedObjId];
            if (ImGui::Button("Save as Prefab"))
            {
                pFw->_engine._assetManager._serializer._vPrefabs.push_back(GO);
            }
            // Name Editor
            {
                static std::string password = GO._name;
                ImGui::InputText("Name", (char*)password.c_str(), IM_ARRAYSIZE(password.c_str()));
                ImGui::SameLine();
                if (ImGui::Button("Confirm"))
                {

                    GO._name = password;
                }
            }

            // Transform Edits
            if (ImGui::TreeNode("Transform Editor"))
            {
                if (ImGui::Button("Move to Origin"))
                {
                    GO.position = v2(0);
                }
                dragVec2f("Velocity", GO.velocity, 0.01f, -1, 1);
                dragTransform(GO);
                ImGui::TreePop();
            }
            Spacing(25);

            if (ImGui::TreeNode("Texture settings"))
            {
                ImGui::Text("Current Texture ID - %s", GO.texID.c_str());

                for (auto kv : pFw->_engine._assetManager._texMap)
                {
                    if (ImGui::TreeNode(kv.first.c_str()))
                    {
                        GO.texID = kv.first;
                        GO.pTexImg = pFw->_engine._assetManager.getTexture(GO.texID);
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Animator settings"))
            {
                ImGui::Text("Current Frame - %i", GO._animator._currentFrame);
                ImGui::Text("Current Animation - %s", GO._animator.pCurrentAnim->name.c_str());

                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::DragInt("Animation Speed (s)", &GO._animator.pCurrentAnim->speed, 1, 0, INT32_MAX, "%d");

                if (ImGui::TreeNode("Change Animation"))
                {
                    static int selectedAnimId = 0;
                    int count = 0;
                    for (auto& anim : pFw->_engine._assetManager._animationMap)
                    {
                        std::string lbl = anim.first;

                        if (ImGui::Selectable(lbl.c_str(), selectedAnimId == count))
                        {
                            selectedAnimId = count;
                            GO._animator.pCurrentAnim = &anim.second;
                        }
                        ++count;
                    }
                    ImGui::TreePop();
                }
                if (ImGui::Button("Play Animation"))
                {
                    GO._animator.play = true;
                }
                ImGui::SameLine();
                if (ImGui::Button("Stop Animation"))
                {
                    GO._animator.play = false;
                }




                ImGui::TreePop();
            }


            if (ImGui::TreeNode("Hitbox Editor"))
            {
                ImGui::Checkbox("Static?", &GO._hitbox->_static);
                ImGui::Checkbox("isTrigger?", &GO._hitbox->_isTrigger);

                ImGui::Text("Change Collider Type");
                ImGui::SameLine();
                if (ImGui::SmallButton("AABB"))
                {
                    GO._hitbox = new Collisions::AABB();
                }
                ImGui::SameLine();
                if (ImGui::SmallButton("Triangle"))
                {
                    GO._hitbox = new Collisions::Triangle();
                }
                ImGui::SameLine();
                if (ImGui::SmallButton("Polygon"))
                {
                    GO._hitbox = new Collisions::Polygon();
                }


                switch (GO._hitbox->_type)
                {
                case Collisions::ColliderType::kAABB:
                    dragAABBHitbox(GO);
                    break;

                case Collisions::ColliderType::kTriangle:
                    dragTriangleHitbox(GO);
                    break;

                case Collisions::ColliderType::kPolygon:
                    break;

                default:
                    break;
                }



                ImGui::TreePop();

            }





        }
    }
    ImGui::End();
    ImGui::End();

    ImGui::Render();

    //End SDL Rendering & ImGui Input
    ImGui::EndFrame();
}

void Renderer::Draw()
{
    pFw->_vScenes[pFw->_sceneIdx].render_objects();

    //SDL_RenderDrawP
   // SDL_RenderDrawPointF(pRenderer, mVPx, mVPy);
    // switch back to master Window target
    SDL_SetRenderTarget(pRenderer, NULL);
    // draws outside the viewport go here

    drawImGui();

    SDL_SetRenderDrawColor(pRenderer, 120, 180, 255, 255);
    SDL_RenderClear(pRenderer);

    //draw imgui here
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), pRenderer);

    // Present the finished image
    SDL_RenderPresent(pRenderer);

}

Renderer::~Renderer()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    SDL_DestroyTexture(pRenderTarget);
    ImGui::DestroyContext();
    SDL_DestroyRenderer(pRenderer);

}
