#include "GUI.h"

#include "RenderContext.h"

#include <ImGui/imgui_impl_sdl2.h>
#include <ImGui/imgui_impl_sdlrenderer2.h>
#include <ImPlot/implot.h>

#pragma region Constructors/Destructor
fro_GENERATED_SINGLETON_CONSTRUCTOR(GUI)
{
	ImGui::CreateContext();

	RenderContext& renderContex{ RenderContext::getInstance() };
	ImGui_ImplSDL2_InitForSDLRenderer(renderContex.getWindow(), renderContex.getRenderer());
	ImGui_ImplSDLRenderer2_Init(renderContex.getRenderer());

	ImGui::StyleColorsClassic();

	ImPlot::CreateContext();
}

fro_GENERATED_SINGLETON_DESTRUCTOR(GUI)
{
	ImPlot::DestroyContext();

	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	ImGui::DestroyContext();
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::GUI::processEvent(const SDL_Event& event) const
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void fro::GUI::startFrame() const
{
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void fro::GUI::endFrame() const
{
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}
#pragma endregion PublicMethods