#if not defined fro_ECS_H
#define fro_ECS_H

#include "ComponentSet.hpp"
#include "ECSGroup.hpp"
#include "Singleton.hpp"
#include "SparseSet.hpp"

#include <algorithm>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace fro
{
	class ECS final : public Singleton<ECS>
	{
	public:
		ECS() = default;

		~ECS() = default;

		GameObjectID createGameObject();

		bool destroyGameObject(GameObjectID const gameObjectID);

		template<typename... ObservedTypes>
		auto& getGroup()
		{
			using Type = ECSGroup<ObservedTypes...>;
			std::type_index const typeIndex{ typeid(Type) };

			auto const ipFoundGroup{ m_umGroups.find(typeIndex) };
			if (ipFoundGroup == m_umGroups.end())
			{
				Type* const pGroup{ new Type{ *this } };
				m_umGroups.emplace(typeIndex, std::unique_ptr<BaseECSGroup>(pGroup));
				return *pGroup;
			}

			return *static_cast<Type* const>(ipFoundGroup->second.get());
		}

		template<typename ComponentType>
		ComponentType* addComponent(GameObjectID const gameObject)
		{
			if (not m_ssGameObjects.contains(gameObject))
				return nullptr;

			auto& componentSet{ getComponentSet<ComponentType>() };
			std::size_t const oldCapacity{ componentSet.getCapacity() };
			ComponentType* const pAddedComponent{ getComponentSet<ComponentType>().addComponent(gameObject) };

			if (pAddedComponent)
				for (auto& [typeIndex, pGroup] : m_umGroups)
					pGroup->onAddComponent(typeid(ComponentType), gameObject, oldCapacity not_eq componentSet.getCapacity());

			return pAddedComponent;
		}

		template<typename ComponentType>
		bool hasComponent(GameObjectID const gameObject)
		{
			return getComponentSet<ComponentType>().hasComponent(gameObject);
		}

		template<typename ComponentType>
		ComponentType* getComponent(GameObjectID const gameObject)
		{
			return getComponentSet<ComponentType>().getComponent(gameObject);
		}

		template<typename ComponentType>
		bool removeComponent(GameObjectID const gameObject)
		{
			bool const result{ getComponentSet<ComponentType>().removeComponent(gameObject) };

			if (result)
				for (auto& [typeIndex, pGroup] : m_umGroups)
					pGroup->onRemoveComponent(typeid(ComponentType), gameObject);

			return result;
		}

	private:
		ECS(ECS const&) = delete;
		ECS(ECS&&) noexcept = delete;

		ECS& operator=(ECS const&) = delete;
		ECS& operator=(ECS&&) noexcept = delete;

		template<typename ComponentType>
		ComponentSet<ComponentType>& getComponentSet()
		{
			std::type_index const typeIndex{ std::type_index(typeid(ComponentType)) };

			if (m_umComponents.contains(typeIndex))
			{
				BaseComponentSet* const pBaseComponentSet{ m_umComponents.at(typeIndex).get() };
				return *static_cast<ComponentSet<ComponentType>*>(pBaseComponentSet);
			}

			auto upComponentSet{ std::make_unique<ComponentSet<ComponentType>>() };
			ComponentSet<ComponentType>* const pComponentSet{ upComponentSet.get() };
			m_umComponents.emplace(typeIndex, std::move(upComponentSet));
			return *pComponentSet;
		}

		std::unordered_map<std::type_index, std::unique_ptr<BaseComponentSet>> m_umComponents{};
		std::unordered_map<std::type_index, std::unique_ptr<BaseECSGroup>> m_umGroups{};

		sparseSet::SparseSet<GameObjectID> m_ssGameObjects{};
		std::vector<GameObjectID> m_vFreeGameObjectIDs{};
	};
}

#endif