#include "froch.hpp"

#include "ComponentManager.hpp"
#include "Idenitifer/IDGenerator.hpp"

namespace fro
{
	EventDispatcher<Entity const, Component const, std::type_index const> ComponentManager::sComponentAttachEvent{};
	EventDispatcher<Entity const, Component const, std::type_index const> ComponentManager::sComponentDetachEvent{};
	std::unordered_map<std::type_index, std::unique_ptr<BaseComponentSparseSet>> ComponentManager::sComponents{};
	std::unordered_map<std::type_index, std::unique_ptr<BaseGroup>> ComponentManager::sGroups{};
}