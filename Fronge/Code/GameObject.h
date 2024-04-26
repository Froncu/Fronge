#pragma once

#include "Defines.hpp"
#include "Transform.h"
#include "Behaviour.h"
#include "Renderable.h"
#include "GUI.h"

#include <memory>
#include <set>
#include <unordered_map>

namespace fro
{
	template<typename ComponentType>
	concept ComponentDerived = std::derived_from<ComponentType, Component>;

	class GameObject final
	{
	public:
		GameObject() = default;

		~GameObject() = default;

		void update() const;
		void render() const;
		void display() const;

		void setParent(GameObject* const pParent, bool keepWorldPosition = true);

		fro_NODISCARD bool owns(const GameObject* const pGameObject) const;
		fro_NODISCARD const GameObject* const getParent() const;
		fro_NODISCARD const std::set<const GameObject*>& getChildren() const;

		template<ComponentDerived ComponentType>
		ComponentType* addComponent() noexcept
		{
			if constexpr (std::same_as<ComponentType, Transform>)
				return nullptr;
			else
			{
				const auto resultPair{ getComponentMap<ComponentType>().emplace(std::make_pair(typeid(ComponentType).hash_code(), new ComponentType(*this))) };
				if (!resultPair.second)
					return nullptr;

				return static_cast<ComponentType*>(resultPair.first->second.get());
			}
		}

		template<ComponentDerived ComponentType>
		bool removeComponent() noexcept
		{
			if constexpr (std::same_as<ComponentType, Transform>)
				return false;
			else
				return getComponentMap<ComponentType>().erase(typeid(ComponentType).hash_code());
		}

		// TODO: this is very slow plus I am not a fan of holding references to components inside other components (fixed for the Transform component)
		template<ComponentDerived ComponentType>
		fro_NODISCARD ComponentType* getComponent() const noexcept
		{
			if constexpr (std::same_as<ComponentType, Transform>)
				return m_pTranform.get();
			else
			{
				const auto& mpComponents{ getComponentMapConstant<ComponentType>() };

				const auto& iterator{ mpComponents.find(typeid(ComponentType).hash_code()) };
				if (iterator == mpComponents.end())
					return nullptr;

				return static_cast<ComponentType*>(iterator->second.get());
			}
		}
		// END TODO

	private:
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) noexcept = delete;

		GameObject& operator=(const GameObject&) = delete;
		GameObject& operator=(GameObject&&) noexcept = delete;

		// HACK: there shouldn't be two methods for getting the component map
		template<ComponentDerived ComponentType>
		fro_NODISCARD const auto& getComponentMapConstant() const
		{
			if constexpr (std::derived_from<ComponentType, Behaviour>)
				return m_mpBehaviours;

			else if constexpr (std::derived_from<ComponentType, Renderable>)
				return m_mpRenderables;

			else if constexpr (std::derived_from<ComponentType, GUI>)
				return m_mpGUIs;

			else
				return m_mpComponents;
		}

		template<ComponentDerived ComponentType>
		fro_NODISCARD auto& getComponentMap()
		{
			if constexpr (std::derived_from<ComponentType, Behaviour>)
				return m_mpBehaviours;

			else if constexpr (std::derived_from<ComponentType, Renderable>)
				return m_mpRenderables;

			else if constexpr (std::derived_from<ComponentType, GUI>)
				return m_mpGUIs;

			else
				return m_mpComponents;
		}
		// END HACK

		std::unique_ptr<Transform> m_pTranform{ new Transform(*this) };

		GameObject* m_pParent{};
		std::set<const GameObject*> m_spChildren{};

		std::unordered_map<std::size_t, std::unique_ptr<Behaviour>> m_mpBehaviours{};
		std::unordered_map<std::size_t, std::unique_ptr<Renderable>> m_mpRenderables{};
		std::unordered_map<std::size_t, std::unique_ptr<GUI>> m_mpGUIs{};
		std::unordered_map<std::size_t, std::unique_ptr<Component>> m_mpComponents{};
	};
}