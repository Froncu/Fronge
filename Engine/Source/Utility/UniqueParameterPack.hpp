#ifndef UNIQUE_PARAMETER_PACK_HPP
#define UNIQUE_PARAMETER_PACK_HPP

#include "froch.hpp"

namespace fro
{
	template<typename...>
	static auto constexpr isUnique{ std::true_type{} };

	template<typename Head, typename... Tail>
	static auto constexpr isUnique<Head, Tail...>
	{
		(not std::is_same_v<Head, Tail> and ...) and isUnique<Tail...>
	};
}

#endif