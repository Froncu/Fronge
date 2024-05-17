#if not defined fro_COMPONENT_SET_H
#define fro_COMPONENT_SET_H

#include "SparseSet.hpp"

#include <optional>
#include <vector>

namespace fro
{
	using GameObjectID = SparseSet<bool>::Key;
	using ComponentIndex = SparseSet<bool>::DataIndex;

	class BaseComponentSet
	{
	public:
		BaseComponentSet() = default;
		BaseComponentSet(BaseComponentSet const&) = default;
		BaseComponentSet(BaseComponentSet&&) noexcept = default;

		virtual ~BaseComponentSet() = default;

		BaseComponentSet& operator=(BaseComponentSet const&) = default;
		BaseComponentSet& operator=(BaseComponentSet&&) noexcept = default;

		virtual bool removeComponent(GameObjectID const gameObjectID) = 0;
	};

	template<typename ComponentType>
	class ComponentSet final : public BaseComponentSet
	{
	public:
		ComponentSet() = default;
		ComponentSet(ComponentSet const&) = default;
		ComponentSet(ComponentSet&&) noexcept = default;

		virtual ~ComponentSet() override = default;

		ComponentSet& operator=(ComponentSet const&) = default;
		ComponentSet& operator=(ComponentSet&&) noexcept = default;
		ComponentType& operator[](GameObjectID const gameObjectID)
		{
			return m_ssComponents[gameObjectID];
		}

		void clear()
		{
			m_ssComponents.clear();
		}

		ComponentType* addComponent(GameObjectID const gameObjectID, ComponentType component = {})
		{
			return m_ssComponents.insert(gameObjectID, component);
		}

		bool hasComponent(GameObjectID const gameObjectID) const
		{
			return m_ssComponents.contains(gameObjectID);
		}

		ComponentType* getComponent(GameObjectID const gameObjectID)
		{
			return m_ssComponents.find(gameObjectID);
		}

		virtual bool removeComponent(GameObjectID const gameObjectID) override
		{
			return m_ssComponents.erase(gameObjectID);
		}

		bool swapComponent(GameObjectID const gameObjectID, ComponentIndex const where)
		{
			m_ssComponents.swap(gameObjectID, where);
		}

	private:
		SparseSet<ComponentType> m_ssComponents{};
	};
}

#endif