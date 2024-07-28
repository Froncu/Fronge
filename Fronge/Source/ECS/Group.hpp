#if not defined GROUP_HPP
#define GROUP_HPP

#include "froch.hpp"

#include "ECS/Components/Component.hpp"
#include "ECS/Entity/Entity.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Reference/Reference.hpp"
#include "Utility/UniqueParameterPack.hpp"

namespace fro
{
	template<std::derived_from<Component>... ObservedComponentTypes>
		requires isUnique<ObservedComponentTypes...>
	class Group final
	{
		using GroupTuple = std::tuple<Reference<Entity const>, Reference<ObservedComponentTypes>...>;

	public:
		// constructing a group can be expensive; preferrably you'd create all your groups
		// before attaching and detaching components
		Group()
		{
			for (auto const& entity : Entity::getAllEntities())
				tryGroup(*entity);
		}

		~Group() = default;

		auto begin() const
		{
			return mGroupedComponents.cbegin();
		}

		auto end() const
		{
			return mGroupedComponents.cend();
		}

	private:
		Group(Group const&) = delete;
		Group(Group&&) noexcept = delete;

		Group& operator=(Group const&) = delete;
		Group& operator=(Group&&) noexcept = delete;

		FRO_NODISCARD bool isObserved(std::type_index const componentTypeIndex)
		{
			static std::array<std::type_index, sizeof...(ObservedComponentTypes)> const observedTypeIndices
			{
				typeid(ObservedComponentTypes)...
			};

			return std::any_of(observedTypeIndices.begin(), observedTypeIndices.end(),
				[componentTypeIndex](std::type_index const observedTypeIndex)
				{
					return componentTypeIndex == observedTypeIndex;
				});
		}

		bool tryGroup(Entity const& entity)
		{
			GroupTuple groupTuple{ entity, entity.findComponent<ObservedComponentTypes>()... };

			if (not (std::get<Reference<ObservedComponentTypes>>(groupTuple).valid() and ...))
				return false;

			mGroupedComponents.emplace_back(std::move(groupTuple));
			return true;
		}

		EventListener<Entity, Component, std::type_index const> mOnComponentAttachEvent
		{
			[this](Entity const& entity, Component const&, std::type_index const& componentTypeIndex)
			{
				if (isObserved(componentTypeIndex))
					return tryGroup(entity);

				return false;
			}, Entity::sComponentAttachEvent
		};

		EventListener<Entity, Component, std::type_index const> mOnComponentDetachEvent
		{
			[this](Entity const& entity, Component const&, std::type_index const& componentTypeIndex)
			{
				if (not isObserved(componentTypeIndex))
					return false;

				auto const invalidGroup
				{
					std::find_if(mGroupedComponents.begin(), mGroupedComponents.end(),
					[&entity](GroupTuple const& groupTuple)
					{
						return entity.getID() == std::get<0>(groupTuple)->getID();
					})
				};

				if (invalidGroup == mGroupedComponents.end())
					return false;

				mGroupedComponents.erase(invalidGroup);
				return true;
			}, Entity::sComponentDetachEvent
		};

		std::vector<GroupTuple> mGroupedComponents{};
	};
}

#endif