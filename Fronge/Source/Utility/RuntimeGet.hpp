#if not defined RUNTIME_GET_HPP
#define RUNTIME_GET_HPP

#include "froch.hpp"

#include "Core.hpp"

namespace fro
{
	template<typename Type1, typename Type2>
	FRO_NODISCARD auto& runtimeGet(std::pair<Type1, Type2>& pair, std::size_t const runtimeIndex)
	{
		return runtimeGet(pair, runtimeIndex, std::make_index_sequence<2>{});
	}

	template<typename... Types>
	FRO_NODISCARD auto& runtimeGet(std::tuple<Types...>& tuple, std::size_t const runtimeIndex)
	{
		return runtimeGet(tuple, runtimeIndex, std::make_index_sequence<std::tuple_size_v<std::tuple<Types...>>>{});
	}

	template<typename... Types>
	FRO_NODISCARD auto& runtimeGet(std::variant<Types...>& variant, std::size_t const runtimeIndex)
	{
		return runtimeGet(variant, runtimeIndex, std::make_index_sequence<std::variant_size_v<std::variant<Types...>>>{});
	}

	template<typename Type>
	concept RuntimeGettable = requires (Type type)
	{
		{ std::get<0>(type) };
	};

	template<RuntimeGettable Type, std::size_t CURRENT_INDEX, std::size_t... NEXT_INDICES>
	FRO_NODISCARD auto& runtimeGet(Type& type, std::size_t const runtimeIndex,
		std::index_sequence<CURRENT_INDEX, NEXT_INDICES...> const)
	{
		if (CURRENT_INDEX == runtimeIndex)
			return std::get<CURRENT_INDEX>(type);

		if constexpr (sizeof...(NEXT_INDICES) not_eq 0)
			return runtimeGet(type, runtimeIndex, std::index_sequence<NEXT_INDICES...>{});
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