#include "ECS.h"

#pragma region PublicMethods
fro::GameObjectID fro::ECS::createGameObject()
{
	if (m_vFreeGameObjectIDs.empty())
		return *m_ssGameObjects.insert(m_ssGameObjects.size());

	GameObjectID const createdGameObjectID{ *m_ssGameObjects.insert(m_vFreeGameObjectIDs.back()) };
	m_vFreeGameObjectIDs.pop_back();
	return createdGameObjectID;
}

bool fro::ECS::destroyGameObject(GameObjectID const gameObjectID)
{
	if (not m_ssGameObjects.erase(gameObjectID))
		return false;

	for (auto&& [typeIndex, pBaseComponentSet] : m_umComponents)
		pBaseComponentSet->removeComponent(gameObjectID);

	m_vFreeGameObjectIDs.push_back(gameObjectID);
	return true;
}
#pragma region PublicMethods