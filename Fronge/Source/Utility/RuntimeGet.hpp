#if not defined RUNTIME_GET_HPP
#define RUNTIME_GET_HPP

#include "froch.hpp"

#include "Core.hpp"

namespace fro
{
	template<typename Type>
	concept RuntimeGettable = requires (Type&& type)
	{
		{ std::get<0>(std::forward<Type>(type)) };
	};

	template<template<typename...> typename RuntimeGettableType, typename... Types>
		requires RuntimeGettable<RuntimeGettableType<Types...>>
	FRO_NODISCARD auto const& runtimeGet(RuntimeGettableType<Types...> const& runtimeGettable, std::size_t const runtimeIndex)
	{
		return runtimeGet(runtimeGettable, runtimeIndex, std::make_index_sequence<sizeof...(Types)>{});
	}

	template<template<typename...> typename RuntimeGettableType, typename... Types>
		requires RuntimeGettable<RuntimeGettableType<Types...>>
	FRO_NODISCARD auto& runtimeGet(RuntimeGettableType<Types...>& runtimeGettable, std::size_t const runtimeIndex)
	{
		return runtimeGet(runtimeGettable, runtimeIndex, std::make_index_sequence<sizeof...(Types)>{});
	}

	template<template<typename...> typename RuntimeGettableType, typename... Types>
		requires RuntimeGettable<RuntimeGettableType<Types...>>
	FRO_NODISCARD auto&& runtimeGet(RuntimeGettableType<Types...>&& runtimeGettable, std::size_t const runtimeIndex)
	{
		return runtimeGet(runtimeGettable, runtimeIndex, std::make_index_sequence<sizeof...(Types)>{});
	}

	template<RuntimeGettable Type, std::size_t CURRENT_INDEX, std::size_t... NEXT_INDICES>
	FRO_NODISCARD auto&& runtimeGet(Type&& type, std::size_t const runtimeIndex,
		std::index_sequence<CURRENT_INDEX, NEXT_INDICES...> const)
	{
		if (CURRENT_INDEX == runtimeIndex)
			return std::get<CURRENT_INDEX>(std::forward<Type>(type));

		if constexpr (sizeof...(NEXT_INDICES) not_eq 0)
			return runtimeGet(std::forward<Type>(type), runtimeIndex, std::index_sequence<NEXT_INDICES...>{});
		else
			FRO_EXCEPTION("runtime index is out of bound!");
	}

	template<typename... Types>
	FRO_NODISCARD std::size_t getParameterPackIndex(std::type_index const typeIndex)
	{
		static std::array<std::type_index, sizeof...(Types)> const typeIndices{ typeid(Types)... };

		std::size_t index{};
		for (; index < typeIndices.size(); ++index)
			if (typeIndices[index] == typeIndex)
				break;

		return index;
	}
}

#endif