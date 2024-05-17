#if not defined fro_ECS_GROUP_H
#define fro_ECS_GROUP_H

#include <iostream>
#include <tuple>
#include <vector>

namespace fro
{
	template<typename Type, typename... Types>
	static void printTypes()
	{
		std::cout << typeid(Type).name();
		if constexpr (sizeof...(Types))
		{
			std::cout << ", ";
			printTypes<Types...>();
		}
		else
			std::cout << '\n';
	}

	template <typename... ComponentTypes>
	using ComponentPack = std::tuple<ComponentTypes...>;

	template<typename OwnedTypePack, typename ObservedTypePack>
	class ECSGroup final {};

	template<typename... OwnedTypes, typename... ObservedTypes>
	class ECSGroup<ComponentPack<OwnedTypes...>, ComponentPack<ObservedTypes...>> final
	{
	public:
		ECSGroup()
		{
			std::cout << "Owned types: ";
			printTypes<OwnedTypes...>();

			std::cout << "Observed types: ";
			printTypes<ObservedTypes...>();
		}
	};

	template<typename... OwnedTypes, typename... ObservedTypes>
	auto createGroup(ComponentPack<ObservedTypes...> const&)
	{
		return ECSGroup<ComponentPack<OwnedTypes...>, ComponentPack<ObservedTypes...>>{};
	}
}

#endif