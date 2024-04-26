#include "GUIContext.h"

#include "RenderContext.h"

#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <implot.h>

#pragma region Constructors/Destructor
fro::GUIContext::GUIContext()
{
	ImGui::CreateContext();

	const RenderContext& renderContext{ RenderContext::getInstance() };
	ImGui_ImplSDL2_InitForSDLRenderer(renderContext.getWindow(), renderContext.getRenderer());
	ImGui_ImplSDLRenderer2_Init(renderContext.getRenderer());

	ImGui::StyleColorsClassic();

	ImPlot::CreateContext();
}

fro::GUIContext::~GUIContext()
{
	ImPlot::DestroyContext();

	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	ImGui::DestroyContext();
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::GUIContext::processSystemEvent(const SDL_Event& event) const
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void fro::GUIContext::startFrame() const
{
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void fro::GUIContext::endFrame() const
{
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}
#pragma endregion PublicMethods