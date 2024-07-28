#if not defined COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP

#include "froch.hpp"

#include "ComponentSparseSet.hpp"
#include "ECS/Entity/Entity.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Utility/SparseSet.hpp"

namespace fro
{
	class ComponentManager final
	{
	public:
		template<ComponentSparseSetStorable ComponentType>
			requires std::default_initializable<ComponentType>
		FRO_NODISCARD static ComponentType& get(Entity& entity)
		{
			auto&& [component, inserted] { getComponentSparseSet<ComponentType>()[entity.getID()] };
			if (inserted)
				sComponentAttachEvent.notify(entity, *component, typeid(ComponentType));

			return component;
		}

		template<ComponentSparseSetStorable ComponentType, typename... ArgumentTypes>
			requires std::constructible_from<ComponentType, ArgumentTypes...>
		static ComponentType* attach(Entity& entity, ArgumentTypes&&... arguments)
		{
			ComponentType* const attachedComponent{
				getSparseSet<ComponentType>().insert(entity.getID(), std::forward<ArgumentTypes>(arguments)...) };

			if (not attachedComponent)
				return attachedComponent;

			sComponentAttachEvent.notify(entity, *attachedComponent, typeid(ComponentType));

			return attachedComponent;
		}

		template<ComponentSparseSetStorable ComponentType>
		static std::optional<ComponentType> detach(Entity& entity)
		{
			std::optional detachedComponent{
				getSparseSet<ComponentType>().erase(entity.getID()) };

			if (not detachedComponent.has_value())
				return detachedComponent;

			sComponentDetachEvent.notify(entity, *detachedComponent, typeid(ComponentType));

			return detachedComponent;
		}

		template<ComponentSparseSetStorable ComponentType>
		FRO_NODISCARD static ComponentType* find(Entity const& entity)
		{
			return getSparseSet<ComponentType>().find(entity.getID());
		}

		template<ComponentSparseSetStorable ComponentType>
		FRO_NODISCARD static bool hasAttached(Entity const& entity)
		{
			return getSparseSet<ComponentType>().contains(entity.getID());
		}

		FRO_API static EventDispatcher<Entity, Component, std::type_index const> sComponentAttachEvent;
		FRO_API static EventDispatcher<Entity, Component, std::type_index const> sComponentDetachEvent;

	private:
		template<ComponentSparseSetStorable ComponentType>
		FRO_NODISCARD static SparseSet<ComponentType>& getSparseSet()
		{
			using ComponentSparseSetType = ComponentSparseSet<ComponentType>;

			auto& baseComponentSparseSet{ sComponents.emplace(typeid(ComponentType), std::make_unique<ComponentSparseSetType>()).first->second };
			return static_cast<ComponentSparseSetType&>(*baseComponentSparseSet).sparseSet;
		}

		FRO_API static std::unordered_map<std::type_index, std::unique_ptr<BaseComponentSparseSet>> sComponents;

		ComponentManager() = delete;
		ComponentManager(ComponentManager const&) = delete;
		ComponentManager(ComponentManager&&) noexcept = delete;

		~ComponentManager() = delete;

		ComponentManager& operator=(ComponentManager const&) = delete;
		ComponentManager& operator=(ComponentManager&&) noexcept = delete;
	};
}

#endif