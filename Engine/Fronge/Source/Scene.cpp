#include "Scene.h"

#pragma region PublicMethods
void fro::Scene::fixedUpdate(float const fixedDeltaSeconds) const
{
	for (auto const& pGameObject : m_vpGameObjects)
		pGameObject->fixedUpdate(fixedDeltaSeconds);
}

void fro::Scene::update(float const deltaSeconds) const
{
	for (auto const& pGameObject : m_vpGameObjects)
		pGameObject->update(deltaSeconds);
}

void fro::Scene::render() const
{
	for (auto const& pGameObject : m_vpGameObjects)
		pGameObject->render();
}

void fro::Scene::display() const
{
	for (auto const& pGameObject : m_vpGameObjects)
		pGameObject->display();
}

fro::GameObject& fro::Scene::addGameObject()
{
	return *m_vpGameObjects.emplace_back(std::make_unique<GameObject>());
}

bool fro::Scene::isActive() const
{
	return m_IsActive;
}
#pragma endregion PublicMethods