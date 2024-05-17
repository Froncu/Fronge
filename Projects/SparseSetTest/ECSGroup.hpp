#if not defined fro_ECS_GROUP_H
#define fro_ECS_GROUP_H

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

		static_assert(isPackUnique<OwnedTypes...>);
		static_assert(isPackUnique<ObservedTypes...>);

		template<typename Type, typename... Pack>
		static auto constexpr isContainedInPack{ (std::is_same_v<Type, Pack> or ...) };

		static_assert(
			(not isContainedInPack<OwnedTypes, ObservedTypes...> and ...) and
			(not isContainedInPack<ObservedTypes, OwnedTypes...> and ...));

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