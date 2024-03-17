#pragma once

#include "Defines.hpp"
#include "Behaviour.h"
#include "Renderable.h"
#include "Component.h"
#include "Transform.h"

#include <memory>
#include <unordered_map>

struct SDL_Renderer;

namespace fro
{
	template<typename ComponentType>
	concept ComponentDerived = std::derived_from<ComponentType, Component>;

	class GameObject final
	{
		friend class Scene;

	public:
		~GameObject() = default;

		template<ComponentDerived ComponentType>
		ComponentType* addComponent() noexcept
		{
			const auto resultPair{ getComponentMap<ComponentType>().emplace(std::make_pair(typeid(ComponentType).hash_code(), new ComponentType(*this))) };
			if (!resultPair.second)
				return nullptr;

			return static_cast<ComponentType*>(resultPair.first->second.get());
		}

		template<ComponentDerived ComponentType>
		bool removeComponent() noexcept
		{
			if constexpr (std::same_as<ComponentType, Transform>)
				return false;
			else
				return getComponentMap<ComponentType>().erase(typeid(ComponentType).hash_code());
		}

		template<ComponentDerived ComponentType>
		fro_NODISCARD_GETTER ComponentType* getComponent() const noexcept
		{
			const auto& mpComponents{ getComponentMap<ComponentType>() };

			const auto& iterator{ mpComponents.find(typeid(ComponentType).hash_code()) };
			if (iterator == mpComponents.end())
				return nullptr;

			return static_cast<ComponentType*>(iterator->second.get());
		}

	private:
		GameObject();
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) noexcept = delete;

		GameObject& operator=(const GameObject&) = delete;
		GameObject& operator=(GameObject&&) noexcept = delete;

		template<ComponentDerived ComponentType>
		auto& getComponentMap() const
		{
			if constexpr (std::derived_from<ComponentType, Behaviour>)
				return m_mpBehaviours;

			else if constexpr (std::derived_from<ComponentType, Renderable>)
				return m_mpRenderables;

			else
				return m_mpComponents;
		}

		void update() const;
		void render(SDL_Renderer* const pRenderer) const;

		std::unordered_map<size_t, std::unique_ptr<Behaviour>> m_mpBehaviours{};
		std::unordered_map<size_t, std::unique_ptr<Renderable>> m_mpRenderables{};
		std::unordered_map<size_t, std::unique_ptr<Component>> m_mpComponents{};
	};
}