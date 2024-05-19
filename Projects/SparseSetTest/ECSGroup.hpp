#if not defined fro_ECS_GROUP_H
#define fro_ECS_GROUP_H

#include <set>
#include <tuple>
#include <typeindex>
#include <unordered_map>

namespace fro
{
	class ECS;

	template <typename... ComponentTypes>
	using ComponentPack = std::tuple<ComponentTypes...>;

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

	template<typename OwnedTypePack, typename ObservedTypePack>
	class ECSGroup final {};

	template<typename... OwnedTypes, typename... ObservedTypes>
	class ECSGroup<ComponentPack<OwnedTypes...>, ComponentPack<ObservedTypes...>> final : public BaseECSGroup
	{
		template<typename...>
		static auto constexpr isPackUnique{ std::true_type{} };

		template<typename Head, typename... Tail>
		static auto constexpr isPackUnique<Head, Tail...>
		{
			(not std::is_same_v<Head, Tail> and ...) and isPackUnique<Tail...>
		};

		static_assert(isPackUnique<OwnedTypes...>, "the specified pack of owned component types is not unique");
		static_assert(isPackUnique<ObservedTypes...>, "the specified pack of observed component types pack is not unique");

		template<typename Type, typename... Pack>
		static auto constexpr isContainedInPack{ (std::is_same_v<Type, Pack> or ...) };

		static_assert(
			(not isContainedInPack<OwnedTypes, ObservedTypes...> and ...) and
			(not isContainedInPack<ObservedTypes, OwnedTypes...> and ...),
			"there is no reason to observe any of the owned component types in the same group");

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
			if (isInPack<OwnedTypes...>(typeIndex))
				m_OwnedMatchingGameObjects[typeIndex].insert(gameObject);

			else if (isInPack<ObservedTypes...>(typeIndex))
				m_ObservedMatchingGameObjects[typeIndex].insert(gameObject);


		}

		virtual void onRemoveComponent(std::type_index const typeIndex, GameObjectID const gameObject) override
		{
			if (isInPack<OwnedTypes...>(typeIndex))
				m_OwnedMatchingGameObjects[typeIndex].erase(gameObject);

			else if (isInPack<ObservedTypes...>(typeIndex))
				m_ObservedMatchingGameObjects[typeIndex].erase(gameObject);


		}

	private:
		using MatchLookup = std::unordered_map<std::type_index, std::set<GameObjectID>>;

		template<typename... Pack>
		bool isInPack(std::type_index const typeIndex)
		{
			return ((std::type_index(typeid(Pack)) == typeIndex) or ...);
		}

		ECS& m_ParentingECS;

		MatchLookup m_OwnedMatchingGameObjects;
		MatchLookup m_ObservedMatchingGameObjects;
	};
}

#endif