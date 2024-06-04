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

fro::Reference<fro::Scene> fro::SceneManager::getScene(std::string_view const name)
{
	auto iFoundScene{ findScene(name) };

	if (iFoundScene == m_vScenes.end())
		return {};

	return *iFoundScene;
}

fro::Reference<fro::Scene> fro::SceneManager::loadScene(std::string name)
{
	auto iFoundScene{ findScene(name) };

	if (iFoundScene not_eq m_vScenes.end())
		return {};

	return m_vScenes.emplace_back(std::move(name));
}

bool fro::SceneManager::unloadScene(std::string name)
{
	auto iFoundScene{ findScene(name) };

	if (iFoundScene == m_vScenes.end())
		return false;

	m_vScenes.erase(iFoundScene);
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
std::vector<fro::Scene>::iterator fro::SceneManager::findScene(std::string_view const name)
{
	return std::find_if(m_vScenes.begin(), m_vScenes.end(),
		[name](Scene const& scene)
		{
			return scene == name;
		});
}
#pragma endregion PrivateMethods