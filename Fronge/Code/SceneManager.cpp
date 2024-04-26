#include "SceneManager.h"

#pragma region PublicMethods
void fro::SceneManager::update() const
{
	for (const auto& pScene : m_vpScenes)
		if (pScene->isActive())
			pScene->update();
}

void fro::SceneManager::render() const
{
	for (const auto& pScene : m_vpScenes)
		if (pScene->isActive())
			pScene->render();
}

void fro::SceneManager::display() const
{
	for (const auto& pScene : m_vpScenes)
		if (pScene->isActive())
			pScene->display();
}

fro::Scene& fro::SceneManager::addScene()
{
	return *m_vpScenes.emplace_back(std::make_unique<Scene>());
}
#pragma endregion PublicMethods