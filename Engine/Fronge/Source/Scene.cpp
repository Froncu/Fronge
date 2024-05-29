#include "Scene.h"

#pragma region PublicMethods
void fro::Scene::fixedUpdate(float const fixedDeltaSeconds) const
{
	for (GameObject const& gameObject : m_vGameObjects)
		gameObject.fixedUpdate(fixedDeltaSeconds);
}

void fro::Scene::update(float const deltaSeconds) const
{
	for (GameObject const& gameObject : m_vGameObjects)
		gameObject.update(deltaSeconds);
}

void fro::Scene::render() const
{
	for (GameObject const& gameObject : m_vGameObjects)
		gameObject.render();
}

void fro::Scene::display() const
{
	for (GameObject const& gameObject : m_vGameObjects)
		gameObject.display();
}

fro::GameObject& fro::Scene::addGameObject()
{
	return m_vGameObjects.emplace_back();
}

bool fro::Scene::isActive() const
{
	return m_IsActive;
}
#pragma endregion PublicMethods