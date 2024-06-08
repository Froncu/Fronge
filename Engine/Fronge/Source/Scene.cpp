#include "Scene.h"

#include <algorithm>

#pragma region Constructors/Destructor
fro::Scene::Scene(std::string name)
	: m_Name{ std::move(name) }
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Scene::awake()
{
	while (m_vGameObjectsToAwake.size())
	{
		if (m_vGameObjectsToAwake.front().valid())
			m_vGameObjectsToAwake.front().get().awake();

		m_vGameObjectsToAwake.erase(m_vGameObjectsToAwake.begin());
	}
}

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

void fro::Scene::cleanUp()
{
	m_GameObjectsToRemove.processAllEvents();
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

	m_vGameObjectsToAwake.emplace_back(m_GameObjects.emplace_back(this, std::move(name)));
	return m_vGameObjectsToAwake.back();
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

	m_vGameObjectsToAwake.emplace_back(m_GameObjects.emplace_back(this, name));
	return m_vGameObjectsToAwake.back();
}

void fro::Scene::removeGameObject(std::string_view const name)
{
	m_GameObjectsToRemove.pushEvent(name);
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