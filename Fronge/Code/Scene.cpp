#include "Scene.h"

#pragma region PublicMethods
void fro::Scene::update() const
{
	for (const auto& pGameObject : m_vpGameObjects)
		pGameObject->update();
}

void fro::Scene::render() const
{
	for (const auto& pGameObject : m_vpGameObjects)
		pGameObject->render();
}

void fro::Scene::display() const
{
	for (const auto& pGameObject : m_vpGameObjects)
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