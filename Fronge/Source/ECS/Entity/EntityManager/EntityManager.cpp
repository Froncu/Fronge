#include "froch.hpp"

#include "EntityManager.hpp"
#include "ECS/Entity/Entity.hpp"

namespace fro
{
	std::unordered_set<Reference<Entity>> const& EntityManager::getAllEntities()
	{
		return getAllEntitiesInternal();
	}

	EventDispatcher<Entity, Component, std::type_index const>& EntityManager::getComponentAttachEvent()
	{
		static EventDispatcher<Entity, Component, std::type_index const> event{};
		return event;
	}

	EventDispatcher<Entity, Component, std::type_index const>& EntityManager::getComponentDetachEvent()
	{
		static EventDispatcher<Entity, Component, std::type_index const> event{};
		return event;
	}

	EventDispatcher<Entity, Scene>& EntityManager::getAddedToSceneEvent()
	{
		static EventDispatcher<Entity, Scene> event{};
		return event;
	}

	EventDispatcher<Entity, Scene>& EntityManager::getRemovedFromSceneEvent()
	{
		static EventDispatcher<Entity, Scene> event{};
		return event;
	}

	std::unordered_set<Reference<Entity>>& EntityManager::getAllEntitiesInternal()
	{
		static std::unordered_set<Reference<Entity>> entities{};
		return entities;
	}

	std::unordered_map<std::type_index, std::unique_ptr<BaseComponentSparseSet>>& EntityManager::getBaseComponentSparseSets()
	{
		static std::unordered_map<std::type_index, std::unique_ptr<BaseComponentSparseSet>> baseComponentSparseSets{};
		return baseComponentSparseSets;
	}
}