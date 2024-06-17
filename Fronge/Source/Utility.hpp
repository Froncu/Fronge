#if not defined UTILITY_HPP
#define UTILITY_HPP

#include "Core.hpp"

#include <cstdint>

namespace fro
{
	using Bitfield = int;
	FRO_NODISCARD static constexpr Bitfield bitfield(std::uint8_t const bitToSet)
	{
		return 1 << bitToSet;
	}
}

#endif