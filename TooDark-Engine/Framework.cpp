#include "Framework.h"
int xyzw[4] = { 0,0,0,0 };
int arr[4] = { 0,0,0,0 };
float xyzwf[4] = { 0,0,0,0 };
void Spacing(int space)
{
    for (int x = 0; x < space; ++x)
    {
        ImGui::Spacing();
    }
}
void dragTransform(GameObject& go)
{
    xyzw[0] = go.position.x;
    xyzw[1] = go.position.y;
    xyzw[2] = go.Scale.x;
    xyzw[3] = go.Scale.y;

    ImGui::DragInt2("Position", xyzw, 1, -1000, 1500, "%d");
    ImGui::DragInt2("Scale", &xyzw[2], 1, -1000, 1500, "%d");

    go.position.x = xyzw[0];
    go.position.y = xyzw[1];
    go.Scale.x = xyzw[2];
    go.Scale.y = xyzw[3];
}
void dragHitbox(GameObject& go)
{
    arr[0] = go._hitbox.extents.x;
    arr[1] = go._hitbox.extents.y;
    arr[2] = go._hitbox.offset.x;
    arr[3] = go._hitbox.offset.y;

    ImGui::DragInt2("Extents", arr, 1, -1000, 1500, "%d");
    Spacing(2);
    ImGui::DragInt2("Offset", &arr[2], 1, -1000, 1500, "%d");

    go._hitbox.extents.x = arr[0];
    go._hitbox.extents.y = arr[1];
    go._hitbox.offset.x = arr[2];
    go._hitbox.offset.y = arr[3];


}
void dragVec2(const char* Label, v2& v, int speed, int min, int max)
{
    xyzw[0] = v.x;
    xyzw[1] = v.y;


    ImGui::DragInt2(Label, xyzw, speed, min, max);

    v.x = xyzw[0];
    v.y = xyzw[1];
}

void dragAABBHitbox(GameObject& go)
{
    auto& hb = go._hitbox;
 
    dragVec2("Extents", hb.extents, 0.5f, -FLT_MAX, FLT_MAX);
    Spacing(3);
    dragVec2("Offset", hb.offset, 0.5f, -FLT_MAX, FLT_MAX);

   
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

Framework::Framework()
{
    init_SDL();
    init_imgui();
}

Framework::~Framework()
{
    cleanup();
}

/// <summary>
/// Initialize SDL Window, Renderer and Render Targets
/// </summary>
void Framework::init_SDL()
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
    pRenderTarget = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_TARGET, 300, 200);
    if (!pRenderTarget)
    {
        SDL_DestroyWindow(pWindow);
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

/// <summary>
/// Initialize Imgui
/// </summary>
void Framework::init_imgui()
{
    // Init imgui (Docking Branch)
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplSDL2_InitForSDLRenderer(pWindow, pRenderer);
    ImGui_ImplSDLRenderer2_Init(pRenderer);

    setImguiSkin();
}
/// <summary>
/// Cleanup for framework, frees all used resources
/// </summary>
void Framework::cleanup()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    SDL_DestroyTexture(pRenderTarget);
    ImGui::DestroyContext();
    SDL_DestroyRenderer(pRenderer);
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
            }
            
        }


        ImGui_ImplSDL2_ProcessEvent(&e);
    }
    SDL_GetMouseState(&mX, &mY);
}

/// <summary>
/// New frame preperation function, Call before issuing draw calls
/// </summary>
void Framework::new_frame()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    SDL_SetRenderTarget(pRenderer, pRenderTarget);


    SDL_SetRenderDrawColor(pRenderer, _BgClearClr[0], _BgClearClr[1], _BgClearClr[2], _BgClearClr[3]);

    SDL_RenderClear(pRenderer);

}

/// <summary>
/// Draw ImGui to SDL Window
/// </summary>

void Framework::draw_imgui()
{
    ImGui::NewFrame();

    // Draws here

    // draw File menu bar
    {
        ImGui::BeginMainMenuBar();

        if (ImGui::BeginMenu("Scene"))
        {
            if (ImGui::MenuItem("New GameObject", "CTRL+N"))
            {
                _vGameObjects.push_back(GameObject(this, "test", "New Object", v2(50, -50)));
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
    ImGui::Text("Mouse Pos - %i,%i", mX, mY);
    


    ImGui::End();   
 
    ImGui::Begin("Game View");
        ImGui::Image((ImTextureID)pRenderTarget, ImGui::GetContentRegionAvail()); 
    ImGui::End();
    static int selectedObjId = -1;

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

            // Name Editor
            {
                static std::string password = GO._name;
                ImGui::InputText("Name", (char*)password.c_str(), IM_ARRAYSIZE(password.c_str()));
                GO._name = password;
            }

            // Transform Edits
            if (ImGui::TreeNode("Transform Editor"))
            {
                if (ImGui::Button("Move to Origin"))
                {
                    GO.position = v2(0);
                }
                dragTransform(GO);
                ImGui::TreePop();
            }
            Spacing(25);

            if (ImGui::TreeNode("Texture settings"))
            {
                ImGui::Text("Current Texture ID - %s", GO.texID.c_str());

                for (auto kv : _engine._assetManager._texMap)
                {
                    if (ImGui::TreeNode(kv.first.c_str()))
                    {
                        GO.texID = kv.first;
                        GO.pTexImg = _engine._assetManager.getTexture(GO.texID);
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
                    for (auto& anim : _engine._assetManager._animationMap)
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
                if( ImGui::Button("Play Animation"))
                {
                    GO._animator.play = true;
                }
                ImGui::SameLine();
                if( ImGui::Button("Stop Animation"))
                {
                    GO._animator.play = false;
                }

          


                ImGui::TreePop();
            }


            if (ImGui::TreeNode("Hitbox Editor"))
            { 
                ImGui::Checkbox("Static?", &GO._static);

                dragHitbox(GO);
                switch (GO._hitbox._type)
                {
                case Collisions::ColliderType::kAABB:
                   
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