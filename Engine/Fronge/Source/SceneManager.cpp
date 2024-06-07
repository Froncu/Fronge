#include "SceneManager.h"

#include <algorithm>

#pragma region PublicMethods
void fro::SceneManager::fixedUpdate(float const fixedDeltaSeconds) const
{
	if (m_ActiveScene.valid())
		m_ActiveScene.get().fixedUpdate(fixedDeltaSeconds);
}

void fro::SceneManager::update(float const deltaSeconds) const
{
	if (m_ActiveScene.valid())
		m_ActiveScene.get().update(deltaSeconds);
}

void fro::SceneManager::lateUpdate(float const deltaSeconds) const
{
	if (m_ActiveScene.valid())
		m_ActiveScene.get().lateUpdate(deltaSeconds);
}

void fro::SceneManager::render() const
{
	if (m_ActiveScene.valid())
		m_ActiveScene.get().render();
}

void fro::SceneManager::display() const
{
	if (m_ActiveScene.valid())
		m_ActiveScene.get().display();
}

bool fro::SceneManager::setActiveScene(Reference<Scene> const scene)
{
	if (not scene.valid())
		return false;

	m_ActiveScene = scene;
	return true;
}

fro::Reference<fro::Scene> fro::SceneManager::addScene(std::string name)
{
	auto const iFoundScene{ findScene(name) };

	if (iFoundScene not_eq m_Scenes.end())
		return {};

	return m_Scenes.emplace_back(std::move(name));
}

fro::Reference<fro::Scene> fro::SceneManager::getScene(std::string_view const name)
{
	auto const iFoundScene{ findScene(name) };

	if (iFoundScene == m_Scenes.end())
		return {};

	return *iFoundScene;
}

fro::Reference<fro::Scene> fro::SceneManager::forceGetScene(std::string const& name)
{
	auto const iFoundScene{ findScene(name) };

	if (iFoundScene not_eq m_Scenes.end())
		return *iFoundScene;

	return m_Scenes.emplace_back(name);
}

bool fro::SceneManager::removeScene(std::string const& name)
{
	auto const iFoundScene{ findScene(name) };

	if (iFoundScene == m_Scenes.end())
		return false;

	m_Scenes.erase(iFoundScene);
	return true;
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
fro::SceneManager::Scenes::iterator fro::SceneManager::findScene(std::string_view const name)
{
	return std::find_if(m_Scenes.begin(), m_Scenes.end(),
		[name](Scene const& scene)
		{
			return name == scene.getName();
		});
}
#pragma endregion PrivateMethods