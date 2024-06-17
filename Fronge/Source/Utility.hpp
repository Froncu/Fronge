#if not defined UTILITY_HPP
#define UTILITY_HPP

#include <cstdint>

namespace fro
{
	using Bitfield = int;
	static constexpr Bitfield bitfield(std::uint8_t const bitToSet)
	{
		return 1 << bitToSet;
	}
}

#endif