#include "ECS.h"

#pragma region PublicMethods
fro::GameObjectID fro::ECS::createGameObject()
{
	if (m_vFreeGameObjectIDs.empty())
		return m_vGameObjectIDs.emplace_back(m_vGameObjectIDs.size());

	m_vGameObjectIDs.push_back(m_vFreeGameObjectIDs.back());
	m_vFreeGameObjectIDs.pop_back();
	return m_vGameObjectIDs.back();
}

bool fro::ECS::destroyGameObject(GameObjectID const gameObjectID)
{
	for (auto&& [typeIndex, pBaseComponentSet] : m_umComponents)
		pBaseComponentSet->removeComponent(gameObjectID);

	auto const iNewEnd{ std::remove(m_vGameObjectIDs.begin(), m_vGameObjectIDs.end(), gameObjectID) };
	if (iNewEnd == m_vGameObjectIDs.end())
		return false;

	m_vGameObjectIDs.assign(m_vGameObjectIDs.begin(), iNewEnd);
	m_vFreeGameObjectIDs.push_back(gameObjectID);
	return true;
}
#pragma region PublicMethods