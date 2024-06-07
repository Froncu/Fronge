#include "Scene.h"

#include <algorithm>

#pragma region Constructors/Destructor
fro::Scene::Scene(std::string name)
	: m_Name{ std::move(name) }
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Scene::fixedUpdate(float const fixedDeltaSeconds) const
{
	for (GameObject const& gameObject : m_GameObjects)
		gameObject.fixedUpdate(fixedDeltaSeconds);
}

void fro::Scene::update(float const deltaSeconds) const
{
	for (GameObject const& gameObject : m_GameObjects)
		gameObject.update(deltaSeconds);
}

void fro::Scene::lateUpdate(float const deltaSeconds) const
{
	for (GameObject const& gameObject : m_GameObjects)
		gameObject.lateUpdate(deltaSeconds);
}

void fro::Scene::render() const
{
	for (GameObject const& gameObject : m_GameObjects)
		gameObject.render();
}

void fro::Scene::display() const
{
	for (GameObject const& gameObject : m_GameObjects)
		gameObject.display();
}

std::string_view fro::Scene::getName() const
{
	return m_Name;
}

fro::Reference<fro::GameObject> fro::Scene::addGameObject(std::string name)
{
	auto const iFoundGameObject{ findGameObject(name) };

	if (iFoundGameObject not_eq m_GameObjects.end())
		return {};

	return m_GameObjects.emplace_back(this, std::move(name));
}

fro::Reference<fro::GameObject> fro::Scene::getGameObject(std::string_view const name)
{
	auto const iFoundGameObject{ findGameObject(name) };

	if (iFoundGameObject == m_GameObjects.end())
		return {};

	return *iFoundGameObject;
}

fro::Reference<fro::GameObject> fro::Scene::forceGetGameObject(std::string const& name)
{
	auto const iFoundGameObject{ findGameObject(name) };

	if (iFoundGameObject not_eq m_GameObjects.end())
		return *iFoundGameObject;

	return m_GameObjects.emplace_back(this, name);
}

bool fro::Scene::removeGameObject(std::string const& name)
{
	auto const iFoundGameObject{ findGameObject(name) };

	if (iFoundGameObject == m_GameObjects.end())
		return false;

	m_GameObjects.erase(iFoundGameObject);
	return true;
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
fro::Scene::GameObjects::iterator fro::Scene::findGameObject(std::string_view const name)
{
	return std::find_if(m_GameObjects.begin(), m_GameObjects.end(),
		[name](Reference<GameObject> const gameObject)
		{
			return name == gameObject.get().getName();
		});
}
#pragma endregion PrivateMethods