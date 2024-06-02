#if not defined fro_GAME_OBJECT_H
#define fro_GAME_OBJECT_H

#include "Behaviour.h"
#include "Defines.hpp"
#include "FixedBehaviour.h"
#include "GUI.h"
#include "BaseReferencable.h"
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

	class GameObject final : public BaseReferencable
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
		Reference<ComponentType> addComponent() noexcept
		{
			static_assert(not std::is_same_v<ComponentType, Transform>,
				"each GameObject receieves a Transform component by default");

			if (m_mpComponents.contains(typeid(ComponentType)))
				return {};

			auto const resultPair{ m_mpComponents.emplace(typeid(ComponentType),
				std::make_unique<ComponentType>(*this)) };

			Reference<ComponentType> const addedComponent
			{
				static_cast<ComponentType* const>(resultPair.first->second.get())
			};

			if constexpr (std::derived_from<ComponentType, FixedBehaviour>)
				m_vFixedBehaviours.push_back(addedComponent);
			if constexpr (std::derived_from<ComponentType, Behaviour>)
				m_vBehaviours.push_back(addedComponent);
			if constexpr (std::derived_from<ComponentType, Renderable>)
				m_vRenderables.push_back(addedComponent);
			if constexpr (std::derived_from<ComponentType, GUI>)
				m_vGUIs.push_back(addedComponent);

			return addedComponent;
		}

		template<ComponentDerived ComponentType>
		bool removeComponent() noexcept
		{
			static_assert(not std::is_same_v<ComponentType, Transform>,
				"each GameObject must contain a Transform component");

			Reference<ComponentType> const foundComponent{ getComponent<ComponentType>() };
			if (not foundComponent.valid())
				return false;

			if constexpr (std::derived_from<ComponentType, FixedBehaviour>)
				m_vFixedBehaviours.erase(
					std::remove(m_vFixedBehaviours.begin(), m_vFixedBehaviours.end(), foundComponent),
					m_vFixedBehaviours.end());
			if constexpr (std::derived_from<ComponentType, Behaviour>)
				m_vBehaviours.erase(
					std::remove(m_vBehaviours.begin(), m_vBehaviours.end(), foundComponent),
					m_vBehaviours.end());
			if constexpr (std::derived_from<ComponentType, Renderable>)
				m_vRenderables.erase(
					std::remove(m_vRenderables.begin(), m_vRenderables.end(), foundComponent),
					m_vRenderables.end());
			if constexpr (std::derived_from<ComponentType, GUI>)
				m_vGUIs.erase(
					std::remove(m_vGUIs.begin(), m_vGUIs.end(), foundComponent),
					m_vGUIs.end());

			return m_mpComponents.erase(typeid(ComponentType));
		}

		template<ComponentDerived ComponentType>
		fro_NODISCARD Reference<ComponentType> getComponent() const noexcept
		{
			auto const iterator{ m_mpComponents.find(typeid(ComponentType)) };
			if (iterator == m_mpComponents.end())
				return {};

			return static_cast<ComponentType* const>(iterator->second.get());
		}

		template<ComponentDerived ComponentType>
		fro_NODISCARD Reference<ComponentType> forceGetComponent() noexcept
		{
			Reference<ComponentType> foundComponent{ findComponent<ComponentType>() };
			if (not foundComponent.valid())
				foundComponent = addComponent<ComponentType>();

			return foundComponent;
		}

	private:
		GameObject(GameObject const&) = delete;

		GameObject& operator=(GameObject const&) = delete;

		std::unordered_map<std::type_index, std::unique_ptr<Component>> m_mpComponents{};

		std::vector<Reference<FixedBehaviour>> m_vFixedBehaviours{};
		std::vector<Reference<Behaviour>> m_vBehaviours{};
		std::vector<Reference<Renderable>> m_vRenderables{};
		std::vector<Reference<GUI>> m_vGUIs{};

		std::set<Reference<GameObject>> m_sChildren{};
		Reference<GameObject> m_Parent{};

		bool m_IsActive{ true };
	};
}

#endif