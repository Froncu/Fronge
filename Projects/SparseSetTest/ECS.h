#if not defined fro_ECS_H
#define fro_ECS_H

#include "ComponentSet.hpp"

#include <algorithm>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace fro
{
	class ECS final
	{
	public:
		ECS() = default;
		ECS(ECS const&) = default;
		ECS(ECS&&) noexcept = default;

		~ECS() = default;

		ECS& operator=(ECS const&) = default;
		ECS& operator=(ECS&&) noexcept = default;

		GameObjectID createGameObject();

		bool destroyGameObject(GameObjectID const gameObjectID);

		template<typename ComponentType>
		ComponentType* addComponent(GameObjectID const gameObject)
		{
			return getComponentSet<ComponentType>().addComponent(gameObject);
		}

		template<typename ComponentType>
		ComponentType* getComponent(GameObjectID const gameObject)
		{
			return getComponentSet<ComponentType>().getComponent(gameObject);
		}

		template<typename ComponentType>
		bool removeComponent(GameObjectID const gameObject)
		{
			return getComponentSet<ComponentType>().removeComponent(gameObject);
		}

	private:
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

		std::vector<GameObjectID> m_vGameObjectIDs{};
		std::vector<GameObjectID> m_vFreeGameObjectIDs{};
		std::unordered_map<std::type_index, std::unique_ptr<BaseComponentSet>> m_umComponents{};
	};
}

#endif