// Written based on Michele "skypjack" Caini's
// blog posts about ECS (https://skypjack.github.io/)
// as well as his EnTT library (https://github.com/skypjack/entt)

#if not defined fro_COMPONENT_SET_H
#define fro_COMPONENT_SET_H

#include <optional>
#include <vector>

namespace fro
{
	template<typename ComponentType>
	class ComponentSet final
	{
	public:
		using GameObject = std::size_t;
		using ComponentIndex = GameObject;
		static ComponentIndex constexpr UNUSED_COMPONENT_INDEX{ std::numeric_limits<ComponentIndex>::max() };

		ComponentSet() = default;
		ComponentSet(ComponentSet const&) = default;
		ComponentSet(ComponentSet&&) noexcept = default;

		~ComponentSet() = default;

		ComponentSet& operator=(ComponentSet const&) = default;
		ComponentSet& operator=(ComponentSet&&) noexcept = default;
		ComponentType& operator[](GameObject const gameObject)
		{
			if (contains(gameObject))
				return naiveFind(gameObject);

			return naiveInsert(gameObject, {});
		}

		void clear()
		{
			m_vSparse.clear();
			m_vDense.clear();
		}

		ComponentType* insert(GameObject const gameObject, ComponentType data = {})
		{
			if (not inSparseRange(gameObject))
				m_vSparse.resize(gameObject + 1, UNUSED_COMPONENT_INDEX);

			else if (naiveContains(gameObject))
				return nullptr;

			return &naiveInsert(gameObject, std::move(data));
		}

		bool contains(GameObject const gameObject) const
		{
			return inSparseRange(gameObject) and naiveContains(gameObject);
		}

		ComponentType* find(GameObject const gameObject)
		{
			if (not contains(gameObject))
				return nullptr;

			return &naiveFind(gameObject);
		}

		bool remove(GameObject const gameObject)
		{
			if (not contains(gameObject))
				return false;

			naiveSwap(gameObject, m_vDense.size() - 1);
			m_vSparse[gameObject] = UNUSED_COMPONENT_INDEX;
			m_vDense.pop_back();

			return true;
		}

		bool swap(GameObject const gameObject, ComponentIndex const where)
		{
			if (not inDenseRange(where) or not contains(gameObject))
				return false;

			naiveSwap(gameObject, where);
			return true;
		}

	private:
		bool inSparseRange(GameObject const gameObject) const
		{
			return gameObject < m_vSparse.size();
		}

		bool inDenseRange(ComponentIndex const componentIndex) const
		{
			return componentIndex < m_vDense.size();
		}

		bool naiveContains(GameObject const gameObject) const
		{
			return m_vSparse[gameObject] not_eq UNUSED_COMPONENT_INDEX;
		}

		ComponentType& naiveFind(GameObject const gameObject)
		{
			return m_vDense[m_vSparse[gameObject]];
		}

		ComponentType& naiveInsert(GameObject const gameObject, ComponentType data)
		{
			m_vSparse[gameObject] = m_vDense.size();

			static_cast<GameObject&>(data) = gameObject;

			m_vDense.push_back(std::move(data));
			return m_vDense.back();
		}

		void naiveSwap(GameObject const gameObject, ComponentIndex const where)
		{
			GameObject const otherGameObject{ m_vDense[where] };
			std::swap(naiveFind(gameObject), m_vDense[where]);
			std::swap(m_vSparse[gameObject], m_vSparse[otherGameObject]);
		}

		std::vector<ComponentIndex> m_vSparse{};
		std::vector<ComponentType> m_vDense{};
	};
}

#endif