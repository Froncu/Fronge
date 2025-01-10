#ifndef BITFIELD_HPP
#define BITFIELD_HPP

#include "froch.hpp"

#include "Core.hpp"

namespace fro
{
	template<std::same_as<int>... BitTypes>
	FRO_NODISCARD std::uint16_t createBitfield(BitTypes const... bits)
	{
		return ((static_cast<std::uint16_t>(1) << static_cast<std::uint16_t>(bits)) | ...);
	}
}

#endif