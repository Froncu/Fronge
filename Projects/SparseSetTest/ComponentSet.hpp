// Written based on Michele "skypjack" Caini's
// blog posts about ECS (https://skypjack.github.io/)
// as well as his EnTT library (https://github.com/skypjack/entt)

#if not defined fro_COMPONENT_SET_H
#define froCOMPONENTE_SET_H

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
			if (not inRange(gameObject))
				m_vSparse.resize(gameObject + 1, UNUSED_COMPONENT_INDEX);

			else if (naiveContains(gameObject))
				return nullptr;

			return &naiveInsert(gameObject, std::move(data));
		}

		bool contains(GameObject const gameObject)
		{
			return inRange(gameObject) and naiveContains(gameObject);
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

			GameObject const lastDataKey{ m_vDense.back() };
			std::swap(naiveFind(gameObject), m_vDense.back());

			m_vSparse[lastDataKey] = m_vSparse[gameObject];
			m_vSparse[gameObject] = UNUSED_COMPONENT_INDEX;

			m_vDense.pop_back();

			return true;
		}

	private:
		bool inRange(GameObject const gameObject)
		{
			return gameObject < m_vSparse.size();
		}

		bool naiveContains(GameObject const gameObject)
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

		std::vector<ComponentIndex> m_vSparse{};
		std::vector<ComponentType> m_vDense{};
	};
}

#endif