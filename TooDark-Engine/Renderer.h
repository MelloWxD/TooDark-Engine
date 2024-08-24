#pragma once
#include"Consts.h"
class Framework;

class Renderer
{
public:
	Renderer(Framework* pFw);

	void init_SDL();
	void init_ImGui();


	void newFrame();
	void drawImGui();
	void Draw();
	~Renderer();

	SDL_Renderer* pRenderer;
	SDL_Texture* pRenderTarget; // Render Target
	SDL_Texture* pRaycastRenderTarget; // 2d Render Target
	SDL_Surface* pViewportSurface; // Render Target

	bool drawGizmos = true;
	v4 _BgClearClr = v4(255, 0, 255, 255);

	Framework* pFw;
};

