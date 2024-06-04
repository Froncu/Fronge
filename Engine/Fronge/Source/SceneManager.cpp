#include "SceneManager.h"

#pragma region PublicMethods
void fro::SceneManager::fixedUpdate(float const fixedDeltaSeconds) const
{
	if (not m_vScenes.empty())
		m_vScenes[m_ActiveSceneIndex].fixedUpdate(fixedDeltaSeconds);
}

void fro::SceneManager::update(float const deltaSeconds) const
{
	if (not m_vScenes.empty())
		m_vScenes[m_ActiveSceneIndex].update(deltaSeconds);
}

void fro::SceneManager::lateUpdate(float const deltaSeconds) const
{
	if (not m_vScenes.empty())
		m_vScenes[m_ActiveSceneIndex].lateUpdate(deltaSeconds);
}

void fro::SceneManager::render() const
{
	if (not m_vScenes.empty())
		m_vScenes[m_ActiveSceneIndex].render();
}

void fro::SceneManager::display() const
{
	if (not m_vScenes.empty())
		m_vScenes[m_ActiveSceneIndex].display();
}

fro::Reference<fro::Scene> fro::SceneManager::addScene(std::string name)
{
	return m_vScenes.emplace_back(std::move(name));
}
#pragma endregion PublicMethods