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
		template<typename Type, typename... Pack>
		struct IsContainedInPack final
		{
		};

		template<typename Type, typename Head, typename... Tail>
		struct IsContainedInPack<Type, Head, Tail...> final
		{
			static bool constexpr value{ std::is_same_v<Type, Head> or IsContainedInPack<Type, Tail...>::value };
		};

		template<typename Type>
		struct IsContainedInPack<Type> final
		{
			static bool constexpr value{};
		};

		template<typename... Pack>
		struct IsPackUnique final
		{
		};

		template<typename Head, typename... Tail>
		struct IsPackUnique<Head, Tail...> final
		{
			static bool constexpr value{ not IsContainedInPack<Head, Tail...>::value and IsPackUnique<Tail...>::value };
		};

		template<typename Type>
		struct IsPackUnique<Type> final
		{
			static bool constexpr value{ true };
		};

		static_assert(IsPackUnique<OwnedTypes...>::value);
		static_assert(IsPackUnique<ObservedTypes...>::value);

	public:
		consteval ECSGroup() = default;
		ECSGroup(ECSGroup const&) = default;
		ECSGroup(ECSGroup&&) noexcept = default;

		~ECSGroup() = default;

		ECSGroup& operator=(ECSGroup const&) = default;
		ECSGroup& operator=(ECSGroup&&) noexcept = default;
	};

	template<typename... OwnedTypes, typename... ObservedTypes>
	consteval auto createGroup(ComponentPack<ObservedTypes...> const&)
	{
		return ECSGroup<ComponentPack<OwnedTypes...>, ComponentPack<ObservedTypes...>>{};
	}
}

#endif