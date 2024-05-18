#if not defined fro_ECS_GROUP_H
#define fro_ECS_GROUP_H

#include <iostream>
#include <tuple>
#include <vector>

namespace fro
{
	template <typename... ComponentTypes>
	using ComponentPack = std::tuple<ComponentTypes...>;

	template<typename OwnedTypePack, typename ObservedTypePack>
	class ECSGroup final {};

	template<typename... OwnedTypes, typename... ObservedTypes>
	class ECSGroup<ComponentPack<OwnedTypes...>, ComponentPack<ObservedTypes...>> final
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
		consteval ECSGroup() = default;
		ECSGroup(ECSGroup const&) = default;
		ECSGroup(ECSGroup&&) noexcept = default;

		~ECSGroup() = default;

		ECSGroup& operator=(ECSGroup const&) = default;
		ECSGroup& operator=(ECSGroup&&) noexcept = default;
	};

	template<typename... OwnedTypes, typename... ObservedTypes>
	consteval auto createGroup(ComponentPack<ObservedTypes...> const& = {})
	{
		return ECSGroup<ComponentPack<OwnedTypes...>, ComponentPack<ObservedTypes...>>{};
	}
}

#endif