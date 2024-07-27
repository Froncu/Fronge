#if not defined GROUP_HPP
#define GROUP_HPP

#include "froch.hpp"

#include "ECS/Component/Component.hpp"
#include "ECS/Entity/Entity.hpp"
#include "Events/Systems/EventListener.hpp"
#include "Reference/Reference.hpp"
#include "Utility/UniqueParameterPack.hpp"

namespace fro
{
	class BaseGroup
	{
	public:
		virtual ~BaseGroup() = default;

	protected:
		BaseGroup() = default;
		BaseGroup(BaseGroup const&) = delete;
		BaseGroup(BaseGroup&&) noexcept = delete;

		BaseGroup& operator=(BaseGroup const&) = delete;
		BaseGroup& operator=(BaseGroup&&) noexcept = delete;
	};

	class ComponentManager;

	template<std::derived_from<Component>... ObservedComponentTypes>
		requires isUnique<ObservedComponentTypes...>
	class Group final : public BaseGroup
	{
		friend ComponentManager;

		using GroupTuple = std::tuple<std::size_t, Reference<ObservedComponentTypes>...>;

	public:
		virtual ~Group() override = default;

		auto begin() const
		{
			return mGroupedComponents.cbegin();
		}

		auto end() const
		{
			return mGroupedComponents.cend();
		}

	private:
		Group()
		{
			for (auto const& entity : Entity::getAllEntities())
				tryGroup(*entity);

			ComponentManager::sComponentAttachEvent.addListener(mOnComponentAttachEvent);
			ComponentManager::sComponentDetachEvent.addListener(mOnComponentDetachEvent);
		}

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
			GroupTuple groupTuple{ entity.getID(), ComponentManager::find<ObservedComponentTypes>(entity)...};

			if (not (std::get<Reference<ObservedComponentTypes>>(groupTuple).valid() and ...))
				return false;

			mGroupedComponents.emplace_back(std::move(groupTuple));
			return true;
		}

		EventListener<Entity const, Component const, std::type_index const> mOnComponentAttachEvent
		{
			[this](Entity const& entity, Component const&, std::type_index const& componentTypeIndex)
			{
				if (isObserved(componentTypeIndex))
					return tryGroup(entity);

				return false;
			}
		};

		EventListener<Entity const, Component const, std::type_index const> mOnComponentDetachEvent
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
						return entity.getID() == std::get<std::size_t>(groupTuple);
					})
				};

				if (invalidGroup not_eq mGroupedComponents.end())
					mGroupedComponents.erase(invalidGroup);

				return true;
			}
		};

		std::vector<GroupTuple> mGroupedComponents{};
	};
}

#endif