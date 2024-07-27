#if not defined COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP

#include "froch.hpp"

#include "ComponentSparseSet.hpp"
#include "ECS/Group.hpp"
#include "Utility/SparseSet.hpp"

namespace fro
{
	class ComponentManager final
	{
	public:
		template<ComponentSparseSetStorable ComponentType>
			requires std::default_initializable<ComponentType>
		FRO_NODISCARD static ComponentType& get(Entity const& entity)
		{
			auto&& [component, inserted] { getComponentSparseSet<ComponentType>()[entity.getID()] };
			if (inserted)
			{
				std::type_index const componentTypeIndex{ typeid(ComponentType) };
				for (auto const& [groupTypeIndex, group] : sGroups)
					group->onComponentAttach(entity, componentTypeIndex);
			}

			return component;
		}

		template<ComponentSparseSetStorable ComponentType, typename... ArgumentTypes>
			requires std::constructible_from<ComponentType, ArgumentTypes...>
		static ComponentType* attach(Entity const& entity, ArgumentTypes&&... arguments)
		{
			ComponentType* const attachedComponent {
				getSparseSet<ComponentType>().insert(entity.getID(), std::forward<ArgumentTypes>(arguments)...)};

			if (not attachedComponent)
				return attachedComponent;

			std::type_index const componentTypeIndex{ typeid(ComponentType) };
			for (auto const& [groupTypeIndex, group] : sGroups)
				group->onComponentAttach(entity, componentTypeIndex);

			return attachedComponent;
		}

		template<ComponentSparseSetStorable ComponentType>
		static std::optional<ComponentType> detach(Entity const& entity)
		{
			std::optional detachedComponent{
				getSparseSet<ComponentType>().erase(entity.getID()) };

			if (not detachedComponent.has_value())
				return detachedComponent;

			std::type_index const componentTypeIndex{ typeid(ComponentType) };
			for (auto const& [groupTypeIndex, group] : sGroups)
				group->onComponentDetach(entity, componentTypeIndex);

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

		template<ComponentSparseSetStorable... ObservedComponentTypes>
			requires isUnique<ObservedComponentTypes...>
		FRO_NODISCARD static Group<ObservedComponentTypes...>& getGroup()
		{
			using GroupType = Group<ObservedComponentTypes...>;

			auto& baseGroup{ sGroups.emplace(typeid(GroupType), std::unique_ptr<GroupType>()).first->second };
			if (not baseGroup.get())
				baseGroup.reset(new GroupType());

			return *static_cast<GroupType* const>(baseGroup.get());
		}

	private:
		template<ComponentSparseSetStorable ComponentType>
		FRO_NODISCARD static SparseSet<ComponentType>& getSparseSet()
		{
			using ComponentSparseSetType = ComponentSparseSet<ComponentType>;

			auto& baseComponentSparseSet{ sComponents.emplace(typeid(ComponentType), std::make_unique<ComponentSparseSetType>()).first->second };
			return static_cast<ComponentSparseSetType&>(*baseComponentSparseSet).sparseSet;
		}

		FRO_API static std::unordered_map<std::type_index, std::unique_ptr<BaseComponentSparseSet>> sComponents;
		FRO_API static std::unordered_map<std::type_index, std::unique_ptr<BaseGroup>> sGroups;

		ComponentManager() = delete;
		ComponentManager(ComponentManager const&) = delete;
		ComponentManager(ComponentManager&&) noexcept = delete;

		~ComponentManager() = delete;

		ComponentManager& operator=(ComponentManager const&) = delete;
		ComponentManager& operator=(ComponentManager&&) noexcept = delete;
	};
}

#endif