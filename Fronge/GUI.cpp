#include "GUI.h"

#include <ImGui/imgui_impl_sdl2.h>
#include <ImGui/imgui_impl_sdlrenderer2.h>

#pragma region PublicMethods
void fro::GUI::initialise(SDL_Window* const pWindow, SDL_Renderer* const pRenderer)
{
	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForSDLRenderer(pWindow, pRenderer);
	ImGui_ImplSDLRenderer2_Init(pRenderer);

	ImGui::StyleColorsClassic();
}

void fro::GUI::destroy()
{
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	ImGui::DestroyContext();
}

void fro::GUI::processEvent(const SDL_Event& event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void fro::GUI::startFrame()
{
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void fro::GUI::endFrame()
{
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}
#pragma endregion PublicMethods