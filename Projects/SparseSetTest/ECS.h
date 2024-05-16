#if not defined fro_ECS_H
#define fro_ECS_H

#include "ComponentSet.hpp"

#include <memory>
#include <typeindex>
#include <unordered_map>

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

		template<typename ComponentType>
		ComponentType* addComponent(std::size_t const gameObject)
		{
			return getComponentSet<ComponentType>().insert(gameObject);
		}

		template<typename ComponentType>
		ComponentType* getComponent(std::size_t const gameObject)
		{
			return getComponentSet<ComponentType>().find(gameObject);
		}

		template<typename ComponentType>
		bool removeComponent(std::size_t const gameObject)
		{
			return getComponentSet<ComponentType>().remove(gameObject);
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

		std::unordered_map<std::type_index, std::unique_ptr<BaseComponentSet>> m_umComponents{};
	};
}

#endif