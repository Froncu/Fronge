#include "SceneManager.h"

#pragma region PublicMethods
void fro::SceneManager::fixedUpdate(float const fixedDeltaSeconds) const
{
	for (Scene const& scene : m_vScenes)
		if (scene.isActive())
			scene.fixedUpdate(fixedDeltaSeconds);
}

void fro::SceneManager::update(float const deltaSeconds) const
{
	for (Scene const& scene : m_vScenes)
		if (scene.isActive())
			scene.update(deltaSeconds);
}

void fro::SceneManager::render() const
{
	for (Scene const& scene : m_vScenes)
		if (scene.isActive())
			scene.render();
}

void fro::SceneManager::display() const
{
	for (Scene const& scene : m_vScenes)
		if (scene.isActive())
			scene.display();
}

fro::Reference<fro::Scene> fro::SceneManager::addScene()
{
	return m_vScenes.emplace_back();
}
#pragma endregion PublicMethods