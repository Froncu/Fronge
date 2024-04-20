#include "SceneManager.h"

#include "Scene.h"
#include "RenderContext.h"
#include "GUI.h"

#include <SDL_render.h>
#include <imgui.h>
#include <implot.h>

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

void fro::SceneManager::update() const
{
	for (const auto& pScene : m_vpScenes)
		if (pScene->m_IsActive)
			pScene->update();
}

void fro::SceneManager::render() const
{
	for (const auto& pScene : m_vpScenes)
		if (pScene->m_IsActive)
			pScene->render();
}

void fro::SceneManager::display() const
{
	for (const auto& pScene : m_vpScenes)
		if (pScene->m_IsActive)
			pScene->display();
}
#pragma endregion PublicMethods