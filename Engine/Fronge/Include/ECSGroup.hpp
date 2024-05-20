#if not defined fro_ECS_GROUP_H
#define fro_ECS_GROUP_H

#include <algorithm>
#include <iostream>
#include <functional>
#include <tuple>
#include <typeindex>
#include <unordered_map>

namespace fro
{
	class BaseECSGroup
	{
		friend class ECS;

	public:
		BaseECSGroup() = default;
		BaseECSGroup(BaseECSGroup const&) = default;
		BaseECSGroup(BaseECSGroup&&) noexcept = default;

		virtual ~BaseECSGroup() = default;

		BaseECSGroup& operator=(BaseECSGroup const&) = default;
		BaseECSGroup& operator=(BaseECSGroup&&) noexcept = default;

	private:
		virtual void onAddComponent(std::type_index const typeIndex, GameObjectID const gameObject, bool reallocated) = 0;
		virtual void onRemoveComponent(std::type_index const typeIndex, GameObjectID const gameObject) = 0;
	};

	template<typename... ObservedTypes>
	class ECSGroup final : public BaseECSGroup
	{
		static_assert<sizeof...(ObservedTypes), "a group that doensn't observe any component types is useless">;

		template<typename...> 
		static auto constexpr isPackUnique{ std::true_type{} };

		template<typename Head, typename... Tail>
		static auto constexpr isPackUnique<Head, Tail...>
		{
			(not std::is_same_v<Head, Tail> and ...) and isPackUnique<Tail...>
		};

		static_assert(isPackUnique<ObservedTypes...>, "the specified pack of observed component types pack is not unique");

	public:
		ECSGroup(ECS& parentingECS)
			: m_ParentingECS{ parentingECS }
		{
		};

		ECSGroup(ECSGroup const&) = delete;
		ECSGroup(ECSGroup&&) noexcept = delete;

		virtual ~ECSGroup() override = default;

		ECSGroup& operator=(ECSGroup const&) = delete;
		ECSGroup& operator=(ECSGroup&&) noexcept = delete;

		auto begin()
		{
			return m_vGroups.begin();
		}

		auto end()
		{
			return m_vGroups.end();
		}

	private:
		using PointersTuple = std::tuple<std::add_pointer_t<ObservedTypes>...>;
		using Group = std::tuple<GameObjectID, std::add_pointer_t<ObservedTypes>...>;

		virtual void onAddComponent(std::type_index const typeIndex, GameObjectID const gameObject, bool reallocated) override
		{
			if (not isInPack<ObservedTypes...>(typeIndex))
				return;

			if (reallocated)
				for (auto& group : m_vGroups)
				{
					GameObjectID groupedGameObject{ std::get<0>(group) };
					((std::get<std::add_pointer_t<ObservedTypes>>(group) = m_ParentingECS.getComponent<ObservedTypes>(groupedGameObject)), ...);
				}

			PointersTuple const pointersTuple{ m_ParentingECS.getComponent<ObservedTypes>(gameObject)... };
			if (not canBeGrouped(pointersTuple))
				return;

			m_vGroups.emplace_back(std::tuple_cat(std::tuple(gameObject), pointersTuple));
		}

		virtual void onRemoveComponent(std::type_index const typeIndex, GameObjectID const gameObject) override
		{
			if (not isInPack<ObservedTypes...>(typeIndex))
				return;

			auto const iNewEnd
			{
				std::remove_if(m_vGroups.begin(), m_vGroups.end(),
					[gameObject](auto const& group)
					{
						return std::get<0>(group) == gameObject;
					})
			};

			m_vGroups.erase(iNewEnd, m_vGroups.end());
		}

		template<typename... Pack>
		static bool isInPack(std::type_index const typeIndex)
		{
			return ((std::type_index(typeid(Pack)) == typeIndex) or ...);
		}

		static bool canBeGrouped(PointersTuple const& tuple)
		{
			return std::apply(
				[](auto... pointers) -> bool
				{
					return (pointers and ...);
				}, tuple);
		}

		ECS& m_ParentingECS;

		std::vector<Group> m_vGroups{};
	};
}

#endif