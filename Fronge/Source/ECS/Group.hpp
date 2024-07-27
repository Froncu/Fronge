#if not defined GROUP_HPP
#define GROUP_HPP

#include "froch.hpp"

#include "ECS/Component/Component.hpp"
#include "ECS/Entity/Entity.hpp"
#include "Reference/Reference.hpp"
#include "Utility/UniqueParameterPack.hpp"

namespace fro
{
	class BaseGroup
	{
	public:
		virtual ~BaseGroup() = default;

		virtual void onComponentAttach(std::size_t const entityID, std::type_index const componentTypeIndex) = 0;
		virtual void onComponentDetach(std::size_t const entityID, std::type_index const componentTypeIndex) = 0;

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

		auto begin()
		{
			return mGroupedComponents.cbegin();
		}

		auto end()
		{
			return mGroupedComponents.cend();
		}

	private:
		Group()
		{
			std::size_t const highestTakenID{ Entity::getHighestTakenID() };

			for (std::size_t entityID{}; entityID <= highestTakenID; ++entityID)
				tryGroup(entityID);
		}

		Group(Group const&) = delete;
		Group(Group&&) noexcept = delete;

		Group& operator=(Group const&) = delete;
		Group& operator=(Group&&) noexcept = delete;

		virtual void onComponentAttach(std::size_t const entityID, std::type_index const componentTypeIndex) override
		{
			if (isObserved(componentTypeIndex))
				tryGroup(entityID);
		}

		virtual void onComponentDetach(std::size_t const entityID, std::type_index const componentTypeIndex) override
		{
			if (not isObserved(componentTypeIndex))
				return;

			auto const invalidGroup
			{
				std::find_if(mGroupedComponents.begin(), mGroupedComponents.end(),
				[entityID](GroupTuple const& groupTuple)
				{
					return entityID == std::get<std::size_t>(groupTuple);
				})
			};

			if (invalidGroup not_eq mGroupedComponents.end())
				mGroupedComponents.erase(invalidGroup);
		}

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

		void tryGroup(std::size_t const entityID)
		{
			GroupTuple groupTuple{ entityID, ComponentManager::find<ObservedComponentTypes>(entityID)... };

			if (not (std::get<Reference<ObservedComponentTypes>>(groupTuple).valid() and ...))
				return;

			mGroupedComponents.emplace_back(std::move(groupTuple));
		}

		std::vector<GroupTuple> mGroupedComponents{};
	};
}

#endif