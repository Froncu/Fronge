#if not defined fro_ECS_GROUP_H
#define fro_ECS_GROUP_H

#include <algorithm>
#include <iostream>
#include <set>
#include <tuple>
#include <typeindex>
#include <unordered_map>

namespace fro
{
	class ECS;

	class BaseECSGroup
	{
	public:
		BaseECSGroup() = default;
		BaseECSGroup(BaseECSGroup const&) = default;
		BaseECSGroup(BaseECSGroup&&) noexcept = default;

		virtual ~BaseECSGroup() = default;

		BaseECSGroup& operator=(BaseECSGroup const&) = default;
		BaseECSGroup& operator=(BaseECSGroup&&) noexcept = default;

		virtual void onAddComponent(std::type_index const typeIndex, GameObjectID const gameObject) = 0;
		virtual void onRemoveComponent(std::type_index const typeIndex, GameObjectID const gameObject) = 0;
	};

	template<typename... ObservedTypes>
	class ECSGroup final : public BaseECSGroup
	{
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

		ECSGroup(ECSGroup const&) = default;
		ECSGroup(ECSGroup&&) noexcept = default;

		virtual ~ECSGroup() override = default;

		ECSGroup& operator=(ECSGroup const&) = default;
		ECSGroup& operator=(ECSGroup&&) noexcept = default;

		virtual void onAddComponent(std::type_index const typeIndex, GameObjectID const gameObject) override
		{
			if (not isInPack<ObservedTypes...>(typeIndex))
				return;

			m_ObservedMap[gameObject] = { m_ParentingECS.getComponent<ObservedTypes>(gameObject)... };
		}

		virtual void onRemoveComponent(std::type_index const typeIndex, GameObjectID const gameObject) override
		{
			if (not isInPack<ObservedTypes...>(typeIndex))
				return;

			m_ObservedMap[gameObject] = { m_ParentingECS.getComponent<ObservedTypes>(gameObject)... };
		}

	private:
		using ObservedTypePointers = std::tuple<std::add_pointer_t<ObservedTypes>...>;

		template<typename... Pack>
		bool isInPack(std::type_index const typeIndex)
		{
			return ((std::type_index(typeid(Pack)) == typeIndex) or ...);
		}

		ECS& m_ParentingECS;

		std::unordered_map<GameObjectID, ObservedTypePointers> m_ObservedMap{};
	};
}

#endif