#include "SceneManager.h"

#include "Scene.h"
#include "RenderContext.h"
#include "GUI.h"

#include <SDL2/SDL_render.h>
#include <ImGui/imgui.h>
#include <ImPlot/implot.h>

#pragma region Constructors/Destructor
fro_GENERATED_SINGLETON_CONSTRUCTOR(SceneManager)
{
}

fro_GENERATED_SINGLETON_DESTRUCTOR(SceneManager)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
fro::Scene& fro::SceneManager::addScene(const std::string& name)
{
	return *m_vpScenes.emplace_back(decltype(m_vpScenes)::value_type{ new Scene(name) });
}

void fro::SceneManager::update()
{
	for (const auto& pScene : m_vpScenes)
		if (pScene->m_IsActive)
			pScene->update();
}

void fro::SceneManager::render()
{
	for (const auto& pScene : m_vpScenes)
		if (pScene->m_IsActive)
			pScene->render();
}

void fro::SceneManager::display()
{
	for (const auto& pScene : m_vpScenes)
		if (pScene->m_IsActive)
			pScene->display();
}
#pragma endregion PublicMethods