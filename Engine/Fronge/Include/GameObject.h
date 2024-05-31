#if not defined fro_GAME_OBJECT_H
#define fro_GAME_OBJECT_H

#include "Behaviour.h"
#include "Defines.hpp"
#include "FixedBehaviour.h"
#include "GUI.h"
#include "References.hpp"
#include "Renderable.h"
#include "Transform.h"

#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>

namespace fro
{
	template<typename ComponentType>
	concept ComponentDerived = std::derived_from<ComponentType, Component>;

	class GameObject final : public Referencable<GameObject>
	{
	public:
		GameObject();
		GameObject(GameObject&& other) noexcept;

		~GameObject() = default;

		GameObject& operator=(GameObject&& other) noexcept;

		void fixedUpdate(float const fixedDeltaSeconds) const;
		void update(float const deltaSeconds) const;
		void render() const;
		void display() const;

		void setActive(bool const isActive);
		void setParent(Reference<GameObject> const parent, bool const keepWorldTransform = true);

		fro_NODISCARD bool owns(Reference<GameObject> const gameObject) const;
		fro_NODISCARD Reference<GameObject> getParent() const;
		fro_NODISCARD std::set<Reference<GameObject>> const& getChildren() const;

		template<ComponentDerived ComponentType>
		ComponentType* addComponent() noexcept
		{
			static_assert(not std::is_same_v<ComponentType, Transform>,
				"each GameObject receieves a Transform component by default");

			if (m_mpComponents.contains(typeid(ComponentType)))
				return nullptr;

			auto const resultPair{ m_mpComponents.emplace(typeid(ComponentType), std::make_unique<ComponentType>(*this)) };
			ComponentType* const pAddedComponent{ static_cast<ComponentType* const>(resultPair.first->second.get()) };

			if constexpr (std::derived_from<ComponentType, FixedBehaviour>)
				m_vpFixedBehaviours.push_back(pAddedComponent);
			if constexpr (std::derived_from<ComponentType, Behaviour>)
				m_vpBehaviours.push_back(pAddedComponent);
			if constexpr (std::derived_from<ComponentType, Renderable>)
				m_vpRenderables.push_back(pAddedComponent);
			if constexpr (std::derived_from<ComponentType, GUI>)
				m_vpGUIs.push_back(pAddedComponent);

			return pAddedComponent;
		}

		template<ComponentDerived ComponentType>
		bool removeComponent() noexcept
		{
			static_assert(not std::is_same_v<ComponentType, Transform>,
				"each GameObject must contain a Transform component");

			Component const* const pFoundComponent{ getComponent<ComponentType>() };
			if (not pFoundComponent)
				return false;

			if constexpr (std::derived_from<ComponentType, FixedBehaviour>)
				m_vpFixedBehaviours.erase(
					std::remove(m_vpFixedBehaviours.begin(), m_vpFixedBehaviours.end(), pFoundComponent),
					m_vpFixedBehaviours.end());
			if constexpr (std::derived_from<ComponentType, Behaviour>)
				m_vpBehaviours.erase(
					std::remove(m_vpBehaviours.begin(), m_vpBehaviours.end(), pFoundComponent),
					m_vpBehaviours.end());
			if constexpr (std::derived_from<ComponentType, Renderable>)
				m_vpRenderables.erase(
					std::remove(m_vpRenderables.begin(), m_vpRenderables.end(), pFoundComponent),
					m_vpRenderables.end());
			if constexpr (std::derived_from<ComponentType, GUI>)
				m_vpGUIs.erase(
					std::remove(m_vpGUIs.begin(), m_vpGUIs.end(), pFoundComponent),
					m_vpGUIs.end());

			return m_mpComponents.erase(typeid(ComponentType));
		}

		template<ComponentDerived ComponentType>
		fro_NODISCARD ComponentType* getComponent() const noexcept
		{
			auto const iterator{ m_mpComponents.find(typeid(ComponentType)) };
			if (iterator == m_mpComponents.end())
				return nullptr;

			return static_cast<ComponentType*>(iterator->second.get());
		}

		template<ComponentDerived ComponentType>
		fro_NODISCARD ComponentType& forceGetComponent() noexcept
		{
			ComponentType* pFoundComponent{ findComponent<ComponentType>() };
			if (not pFoundComponent)
				pFoundComponent = addComponent<ComponentType>();

			return *pFoundComponent;
		}

	private:
		GameObject(GameObject const&) = delete;

		GameObject& operator=(GameObject const&) = delete;

		std::unordered_map<std::type_index, std::unique_ptr<Component>> m_mpComponents{};

		std::vector<FixedBehaviour*> m_vpFixedBehaviours{};
		std::vector<Behaviour*> m_vpBehaviours{};
		std::vector<Renderable*> m_vpRenderables{};
		std::vector<GUI*> m_vpGUIs{};

		std::set<Reference<GameObject>> m_sChildren{};
		Reference<GameObject> m_Parent{};

		bool m_IsActive{ true };
	};
}

#endif