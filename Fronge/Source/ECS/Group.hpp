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
	class Group final : public Referencable
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

		Group(Group const&) = default;
		Group(Group&&) noexcept = default;

		~Group() = default;

		Group& operator=(Group const&) = default;
		Group& operator=(Group&&) noexcept = default;

		auto operator[](std::size_t const index) const
		{
			return mGroupedComponents[index];
		}

		auto begin() const
		{
			return mGroupedComponents.cbegin();
		}

		auto end() const
		{
			return mGroupedComponents.cend();
		}

		std::size_t size() const
		{
			return mGroupedComponents.size();
		}

	private:
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
			[smartThis = Reference{ this }](Entity const& entity, Component const&, std::type_index const& componentTypeIndex)
			{
				if (smartThis->isObserved(componentTypeIndex))
					return smartThis->tryGroup(entity);

				return false;
			}, Entity::getComponentAttachEvent()
		};

		EventListener<Entity, Component, std::type_index const> mOnComponentDetachEvent
		{
			[smartThis = Reference{ this }](Entity const& entity, Component const&, std::type_index const& componentTypeIndex)
			{
				if (not smartThis->isObserved(componentTypeIndex))
					return false;

				auto const newEnd
				{
					std::remove_if(smartThis->mGroupedComponents.begin(), smartThis->mGroupedComponents.end(),
					[&entity](GroupTuple const& groupTuple)
					{
						return entity.getID() == std::get<0>(groupTuple)->getID();
					})
				};

				smartThis->mGroupedComponents.erase(newEnd, smartThis->mGroupedComponents.end());
				return true;
			}, Entity::getComponentDetachEvent()
		};

		std::vector<GroupTuple> mGroupedComponents{};
	};
}

#endif